#include "engine/renderer/renderer.h"

#include <map>

#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::renderMeshes(const Scene& scene) const
{
	if (!_shader.isProgramValid())
		throw std::string("Invalid shader program!");

	_renderOpaqueMeshInstances();
	_enableTranslucentRendering();

	RendererData::translucentMeshInstances_t sortedTranslucentMeshInstances = RendererData::translucentMeshInstances_t();
	_sortTranslucentMeshInstancesInto(scene, sortedTranslucentMeshInstances);
	_renderTranslucentMeshInstances(sortedTranslucentMeshInstances);
	_enableOpaqueRendering();
}




void Renderer::_enableOpaqueRendering() const
{
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}


void Renderer::_enableTranslucentRendering() const
{
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void Renderer::_sortTranslucentMeshInstancesInto(const Scene& scene, RendererData::translucentMeshInstances_t& sortedTranslucentMeshInstancesOut) const
{
	Coords3f cameraPosition;
	Quaternion cameraRotation;
	Coords3f cameraSize;
	Transform::decomposeTransformMatrix(scene.activeCamera(), cameraPosition, cameraRotation, cameraSize);

	auto translucentMeshInstancesToDistance = std::map<const std::pair<const MeshComponent* const, const TransformComponent*>*, float>();
	for (const auto& translucentMeshInstance : _translucentMeshInstances)
	{
		const MeshComponent& mesh = *translucentMeshInstance.first;
		const TransformComponent& transform = *translucentMeshInstance.second;
		if (!mesh.enabled())
			continue;

		Coords3f instancePosition;
		Quaternion instanceRotation;
		Coords3f instanceSize;
		Transform::decomposeTransformMatrix(transform, instancePosition, instanceRotation, instanceSize);
		float distance = cameraPosition.distance(instancePosition);
		translucentMeshInstancesToDistance[&translucentMeshInstance] = distance;
	}
	
	auto firstIterator = translucentMeshInstancesToDistance.begin();
	while (firstIterator != translucentMeshInstancesToDistance.cend())
	{
		const std::pair<const MeshComponent* const, const TransformComponent*>* nearestMesh = firstIterator->first;
		float minDistance = firstIterator->second;

		auto secondIterator = std::next(firstIterator);
		for (; secondIterator != translucentMeshInstancesToDistance.cend(); secondIterator++)
		{
			const std::pair<const MeshComponent* const, const TransformComponent*>* mesh = secondIterator->first;
			float distance = secondIterator->second;

			if (distance < minDistance)
			{
				nearestMesh = mesh;
				minDistance = distance;
			}
		}

		firstIterator++;
		if (firstIterator != translucentMeshInstancesToDistance.cend() && firstIterator->first == nearestMesh)
			firstIterator--;

		sortedTranslucentMeshInstancesOut[nearestMesh->first] = nearestMesh->second;
		translucentMeshInstancesToDistance.erase(nearestMesh);
	}
}



void Renderer::_renderOpaqueMeshInstances() const
{
	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	for (const auto& meshIterator : _opaqueMeshInstances)
	{
		const MeshData* meshData = meshIterator.first;
		const std::unordered_set<const TransformComponent*> transformComponents = meshIterator.second;

		if (!meshData->enabled())
			continue;

		_submitMeshData(*meshData);
		for (const auto& transform : transformComponents)
		{
			if (instanceBuffer.nInstances >= RendererSettings::MAX_INSTANCES_PER_SUBMISSION)
				_submitRenderableData(*meshData, instanceBuffer);

			_addToInstanceBuffer(instanceBuffer, transform);
		}

		_submitRenderableData(*meshData, instanceBuffer);
	}
}


void Renderer::_renderTranslucentMeshInstances(const RendererData::translucentMeshInstances_t& translucentMeshInstances) const
{
	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	auto meshIterator = translucentMeshInstances.cbegin();
	while (meshIterator != translucentMeshInstances.cend())
	{
		const MeshComponent* originalMesh = meshIterator->first;

		_submitMeshData(originalMesh->meshData());
		while (meshIterator != translucentMeshInstances.cend() && meshIterator->first == originalMesh)
		{
			if (instanceBuffer.nInstances >= RendererSettings::MAX_INSTANCES_PER_SUBMISSION)
				_submitRenderableData(*originalMesh, instanceBuffer);

			const TransformComponent* transform = meshIterator->second;
			_addToInstanceBuffer(instanceBuffer, transform);
			meshIterator++;
		}

		_submitRenderableData(*originalMesh, instanceBuffer);
	}
}



void Renderer::_addToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, const TransformComponent* transform) const
{
	_applyTransform(*transform);
	memcpy(instanceBuffer.pvmMatrix[instanceBuffer.nInstances],    mCompMatrix[PROJ_VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.vmMatrix[instanceBuffer.nInstances],     mCompMatrix[VIEW_MODEL],		 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.normalMatrix[instanceBuffer.nInstances], mNormal3x3,					 3 * 3 * sizeof(float));
	instanceBuffer.nInstances++;
}


void Renderer::_applyTransform(const TransformComponent& transform) const
{
	pushMatrix(MODEL);
	loadMatrix(MODEL, transform.transformMatrix());
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	computeNormalMatrix3x3();
}



void Renderer::_submitRenderableData(const MeshData& meshData, RendererData::SubmitInstanceBuffer& instanceBuffer) const
{
	_submitInstanceBuffer(instanceBuffer);
	_renderMesh(meshData, instanceBuffer);
	instanceBuffer.nInstances = 0;
}


void Renderer::_submitMeshData(const MeshData& meshData) const
{
	const Material& material = meshData.material();
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_AMBIENT],  1, material.ambient);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_DIFFUSE],  1, material.diffuse);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_SPECULAR], 1, material.specular);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_EMISSIVE], 1, material.emissive);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_SHININESS],    material.shininess);

	const Texture& texture = meshData.texture();
	glUniform1ui(_uniformLocation[RendererData::ShaderUniformType::N_TEXTURES],   (unsigned int)texture.nTextures());
	glUniform1ui(_uniformLocation[RendererData::ShaderUniformType::TEXTURE_MODE], (unsigned int)texture.textureMode());
	glUniform2ui(_uniformLocation[RendererData::ShaderUniformType::TEXTURE_MAPS], texture.textureIds()[0], texture.textureIds()[1]);
}


void Renderer::_submitInstanceBuffer(const RendererData::SubmitInstanceBuffer& instanceBuffer) const
{
	glUniformMatrix4fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_PVM_MATRIX],    instanceBuffer.nInstances, GL_FALSE, (const float*)instanceBuffer.pvmMatrix);
	glUniformMatrix4fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_VM_MATRIX],     instanceBuffer.nInstances, GL_FALSE, (const float*)instanceBuffer.vmMatrix);
	glUniformMatrix3fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_NORMAL_MATRIX], instanceBuffer.nInstances, GL_FALSE, (const float*)instanceBuffer.normalMatrix);
}


void Renderer::_renderMesh(const MeshData& mesh, RendererData::SubmitInstanceBuffer& instanceBuffer) const
{
	const MyMesh& meshData = mesh.mesh();
	glBindVertexArray(meshData.vao);
	glDrawElementsInstanced(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0, instanceBuffer.nInstances);
	glBindVertexArray(0);
}