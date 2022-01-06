#include "engine/renderer/renderer.h"

#include "engine/renderer/mesh/geometry.h"
#include "engine/renderer/mesh/texture.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::renderMeshes(const Scene& scene) const
{
	RendererData::SubmitInstanceData renderableInstanceBuffer = RendererData::SubmitInstanceData();
	for (const auto& meshIterator : _solidMeshInstances)
	{
		const MeshData* meshData = meshIterator.first;
		const std::unordered_set<const TransformComponent*> transformComponents = meshIterator.second;

		if (!meshData->enabled())
			continue;

		_submitMeshData(*meshData);

		for (const auto& transform : transformComponents)
		{
			if (renderableInstanceBuffer.nRenderableInstances >= RendererSettings::MAX_RENDERABLE_INSTANCES_SUBMISSION)
				_submitRenderableData(*meshData, renderableInstanceBuffer);

			_formatRenderableInstanceBuffer(renderableInstanceBuffer, transform);
		}	

		_submitRenderableData(*meshData, renderableInstanceBuffer);
	}
}




void Renderer::_formatRenderableInstanceBuffer(RendererData::SubmitInstanceData& renderableInstanceBuffer, const TransformComponent* transform) const
{
	_applyTransform(*transform);

	memcpy(renderableInstanceBuffer.pvmMatrix[renderableInstanceBuffer.nRenderableInstances], mCompMatrix[PROJ_VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(renderableInstanceBuffer.vmMatrix[renderableInstanceBuffer.nRenderableInstances], mCompMatrix[VIEW_MODEL], 4 * 4 * sizeof(float));
	memcpy(renderableInstanceBuffer.normalMatrix[renderableInstanceBuffer.nRenderableInstances], mNormal3x3, 3 * 3 * sizeof(float));

	renderableInstanceBuffer.nRenderableInstances++;
}


void Renderer::_applyTransform(const TransformComponent& transform) const
{
	pushMatrix(MODEL);
	loadMatrix(MODEL, transform.transformMatrix());
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	computeNormalMatrix3x3();
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


void Renderer::_submitRenderableInstanceBuffer(const RendererData::SubmitInstanceData& renderableInstanceBuffer) const
{
	glUniformMatrix4fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_PVM_MATRIX], 
		renderableInstanceBuffer.nRenderableInstances, GL_FALSE, (const float*)renderableInstanceBuffer.pvmMatrix);
	glUniformMatrix4fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_VM_MATRIX], 
		renderableInstanceBuffer.nRenderableInstances, GL_FALSE, (const float*)renderableInstanceBuffer.vmMatrix);
	glUniformMatrix3fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_NORMAL_MATRIX], 
		renderableInstanceBuffer.nRenderableInstances, GL_FALSE, (const float*)renderableInstanceBuffer.normalMatrix);
}


void Renderer::_submitRenderableData(const MeshData& meshData, RendererData::SubmitInstanceData& renderableInstanceBuffer) const
{
	_submitRenderableInstanceBuffer(renderableInstanceBuffer);
	_renderInstanceBuffer(meshData, renderableInstanceBuffer);
	renderableInstanceBuffer.nRenderableInstances = 0;
}




void Renderer::_renderInstanceBuffer(const MeshData& mesh, RendererData::SubmitInstanceData& renderableInstanceBuffer) const
{
	const MyMesh& meshData = mesh.mesh();

	glBindVertexArray(meshData.vao);

	if (!_shader.isProgramValid())
		throw std::string("Invalid shader program!");

	glDrawElementsInstanced(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0, renderableInstanceBuffer.nRenderableInstances);
	glBindVertexArray(0);
}