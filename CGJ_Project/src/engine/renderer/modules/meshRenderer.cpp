#include "engine/renderer/renderer.h"

#include <unordered_map>
#include <numeric>

#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::_renderMeshes(const Scene& scene, RendererSettings::RendererMode renderMode) const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)renderMode);
	_renderOpaqueMeshInstances();

	_initTranslucentRendering();
	RendererData::translucentMeshInstances_t sortedTranslucentMeshInstances = RendererData::translucentMeshInstances_t();
	_sortTranslucentMeshInstancesInto(scene, sortedTranslucentMeshInstances);
	_renderTranslucentMeshInstances(sortedTranslucentMeshInstances);
	_terminateTranslucentRendering();
}




void Renderer::_initTranslucentRendering() const
{
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Renderer::_terminateTranslucentRendering() const
{
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}


void Renderer::_sortTranslucentMeshInstancesInto(const Scene& scene, RendererData::translucentMeshInstances_t& sortedTranslucentMeshInstancesOut) const
{
	Coords3f cameraPosition;
	Quaternion cameraRotation;
	Coords3f cameraSize;
	Transform::decomposeTransformMatrix(scene.activeCamera(), cameraPosition, cameraRotation, cameraSize);

	auto translucentMeshInstancesToDistance = std::unordered_map<const std::pair<const MeshComponent* const, const TransformComponent*>*, float>();
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
	

	while (translucentMeshInstancesToDistance.size() > 0)
	{
		auto translucentMeshIterator = translucentMeshInstancesToDistance.cbegin();
		if (translucentMeshIterator == translucentMeshInstancesToDistance.cend())
			break;

		const std::pair<const MeshComponent* const, const TransformComponent*>* nearestMesh = translucentMeshIterator->first;
		float minDistance = translucentMeshIterator->second;

		for (; translucentMeshIterator != translucentMeshInstancesToDistance.cend(); translucentMeshIterator++)
		{
			const std::pair<const MeshComponent* const, const TransformComponent*>* mesh = translucentMeshIterator->first;
			float distance = translucentMeshIterator->second;

			if (distance < minDistance)
			{
				nearestMesh = mesh;
				minDistance = distance;
			}
		}

		sortedTranslucentMeshInstancesOut[nearestMesh->first] = nearestMesh->second;
		translucentMeshInstancesToDistance.erase(nearestMesh);
	}
}




void Renderer::_addObjectToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, const TransformComponent* transform) const
{ 
	loadMatrix(MODEL, _modelTransforms.preModelTransform);
	multMatrix(MODEL, transform->transformMatrix());
	if (_modelTransforms.applyPostMatrix) 
		multMatrix(MODEL, _modelTransforms.postModelTransform);

	computeDerivedMatrix(PROJ_VIEW_MODEL);
	computeNormalMatrix3x3();

	memcpy(instanceBuffer.pvmMatrix[instanceBuffer.nInstances],    mCompMatrix[PROJ_VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.vmMatrix[instanceBuffer.nInstances],     mCompMatrix[VIEW_MODEL],		 4 * 4 * sizeof(float));
	memcpy(instanceBuffer.normalMatrix[instanceBuffer.nInstances], mNormal3x3,					 3 * 3 * sizeof(float));
	instanceBuffer.nInstances++;
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
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_AMBIENT],  1, material.ambient);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_DIFFUSE],  1, material.diffuse);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_SPECULAR], 1, material.specular);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_EMISSIVE], 1, material.emissive);
	glUniform1f(_uniformLocator[RendererUniformLocations::MATERIAL_SHININESS],    material.shininess);

	const Texture& texture = meshData.texture();
	glUniform1ui(_uniformLocator[RendererUniformLocations::TEXTURE_MODE], (unsigned int)texture.textureMode());
	glUniform1ui(_uniformLocator[RendererUniformLocations::TEXTURE_N_COLOR_MAPS], (unsigned int)texture.nColorMaps());
	glUniform1ui(_uniformLocator[RendererUniformLocations::TEXTURE_N_NORMAL_MAPS],   (unsigned int)texture.nNormalMaps());
	glUniform1uiv(_uniformLocator[RendererUniformLocations::TEXTURE_COLOR_MAP_IDS], RendererSettings::MAX_TEXTURES_PER_MESH, texture.colorMapIds());
	glUniform1uiv(_uniformLocator[RendererUniformLocations::TEXTURE_NORMAL_MAP_IDS], RendererSettings::MAX_TEXTURES_PER_MESH, texture.normalMapIds());
}


void Renderer::_submitInstanceBuffer(const RendererData::SubmitInstanceBuffer& instanceBuffer) const
{
	glUniformMatrix4fv(_uniformLocator[RendererUniformLocations::INSTANCE_PVM_MATRIX],    instanceBuffer.nInstances, GL_FALSE, (const float*)instanceBuffer.pvmMatrix);
	glUniformMatrix4fv(_uniformLocator[RendererUniformLocations::INSTANCE_VM_MATRIX],     instanceBuffer.nInstances, GL_FALSE, (const float*)instanceBuffer.vmMatrix);
	glUniformMatrix3fv(_uniformLocator[RendererUniformLocations::INSTANCE_NORMAL_MATRIX], instanceBuffer.nInstances, GL_FALSE, (const float*)instanceBuffer.normalMatrix);

	if (instanceBuffer.usesParticles)
		glUniform4fv(_uniformLocator[RendererUniformLocations::INSTANCE_PARTICLE_COLOR], instanceBuffer.nInstances, (const float*)instanceBuffer.particleColor);
}


void Renderer::_renderMesh(const MeshData& mesh, RendererData::SubmitInstanceBuffer& instanceBuffer) const
{
	const MyMesh& meshData = mesh.mesh();
	glBindVertexArray(meshData.vao);
	glDrawElementsInstanced(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0, instanceBuffer.nInstances);
	glBindVertexArray(0);
}




void Renderer::_renderOpaqueMeshInstances() const
{
	for (const auto& meshIterator : _opaqueMeshInstances)
	{
		RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
		const MeshData* meshData = meshIterator.first;
		const std::unordered_map<const MeshComponent*, const TransformComponent*>& meshInstances = meshIterator.second;
		_submitMeshData(*meshData);

		for (const auto& meshInstanceIterator : meshInstances)
		{
			const MeshComponent* mesh = meshInstanceIterator.first;
			const TransformComponent* transform = meshInstanceIterator.second;
			if (!mesh->enabled())
				continue;

			if (instanceBuffer.nInstances >= RendererSettings::MAX_INSTANCES_PER_SUBMISSION)
				_submitRenderableData(*meshData, instanceBuffer);

			_addObjectToInstanceBuffer(instanceBuffer, transform);
		}

		_submitRenderableData(*meshData, instanceBuffer);
	}
}


void Renderer::_renderTranslucentMeshInstances(const RendererData::translucentMeshInstances_t& translucentMeshInstances) const
{
	auto meshIterator = translucentMeshInstances.cbegin();
	while (meshIterator != translucentMeshInstances.cend())
	{
		RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
		const MeshComponent* originalMesh = meshIterator->first;
		_submitMeshData(originalMesh->meshData());

		while (meshIterator != translucentMeshInstances.cend() && meshIterator->first == originalMesh)
		{
			if (instanceBuffer.nInstances >= RendererSettings::MAX_INSTANCES_PER_SUBMISSION)
				_submitRenderableData(*originalMesh, instanceBuffer);

			const MeshComponent* mesh = meshIterator->first;
			const TransformComponent* transform = meshIterator->second;
			if (mesh->enabled())
				_addObjectToInstanceBuffer(instanceBuffer, transform);
			meshIterator++;
		}

		_submitRenderableData(*originalMesh, instanceBuffer);
	}
}