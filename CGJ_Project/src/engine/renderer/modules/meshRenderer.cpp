#include "engine/renderer/renderer.h"

#include "engine/renderer/geometry.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::renderMeshes(const Scene& scene) const
{
	const auto& meshComponents = scene.getSceneComponents<MeshComponent>();
	for (const auto& meshIterator : meshComponents)
	{
		EntityHandle entityId = meshIterator.first;
		const MeshComponent& mesh = meshIterator.second;
		if (!mesh.enabled())
			continue;

		const Entity& entity = scene.getEntityById(entityId);
		_loadMesh(mesh);
		_loadTexture(mesh);
		_applyTransform(entity);
		_renderMesh(mesh);
	}
}




void Renderer::_loadMesh(const MeshComponent& mesh) const
{
	const MyMesh& meshData = mesh.meshData();
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_AMBIENT], 1, meshData.mat.ambient);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_DIFFUSE], 1, meshData.mat.diffuse);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_SPECULAR], 1, meshData.mat.specular);
	glUniform1f(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_SHININESS], meshData.mat.shininess);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_EMISSIVE], 1, meshData.mat.emissive);
	glUniform1ui(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_TEXCOUNT], meshData.mat.texCount);
}


void Renderer::_loadTexture(const MeshComponent& mesh) const
{
	constexpr Renderer::ShaderUniformType textureMaps[] = {
		ShaderUniformType::TEX_MAP_0,
		ShaderUniformType::TEX_MAP_1,
		ShaderUniformType::TEX_MAP_2
	};

	const MeshComponent::Texture* textureData = mesh.texturePtr();
	if (textureData != nullptr)
	{
		glUniform1ui(_uniformLocation[Renderer::ShaderUniformType::TEX_MODE], (unsigned int)textureData->textureMode());
		for (size_t i = 0; i < textureData->nTextures(); i++)
			glUniform1ui(_uniformLocation[textureMaps[i]], textureData->textureIds()[i]);
	}
	else
	{
		glUniform1i(_uniformLocation[Renderer::ShaderUniformType::TEX_MODE], (int)MeshComponent::TextureMode::TEXEL_COLOR);
	}	
}


void Renderer::_applyTransform(const Entity& entity) const
{
	pushMatrix(MODEL);
	loadMatrix(MODEL, entity.transform().transformMatrix());
}


void Renderer::_renderMesh(const MeshComponent& mesh) const
{
	const MyMesh& meshData = mesh.meshData();

	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::VM], 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::PVM], 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(_uniformLocation[Renderer::ShaderUniformType::NORMAL], 1, GL_FALSE, mNormal3x3);

	glBindVertexArray(meshData.vao);

	if (!_shader.isProgramValid())
		throw std::string("Invalid shader program!");

	glDrawElements(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	popMatrix(MODEL);
}