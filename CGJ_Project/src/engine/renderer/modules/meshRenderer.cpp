#include "engine/renderer/renderer.h"

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

	_renderMeshInstances(_solidMeshInstances);
}




void Renderer::_renderMeshInstances(const RendererData::meshInstances_t& meshInstances) const
{
	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	for (const auto& meshIterator : meshInstances)
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