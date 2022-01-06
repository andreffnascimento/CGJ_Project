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
	const Material& material = mesh.material();
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_AMBIENT], 1, material.ambient);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_DIFFUSE], 1, material.diffuse);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_SPECULAR], 1, material.specular);
	glUniform1f(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_SHININESS], material.shininess);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::MATERIAL_EMISSIVE], 1, material.emissive);
}


void Renderer::_loadTexture(const MeshComponent& mesh) const
{
	const MeshComponent::Texture& texture = mesh.texture();
	glUniform1ui(_uniformLocation[Renderer::ShaderUniformType::N_TEXTURES], (unsigned int)texture.nTextures());
	glUniform1ui(_uniformLocation[Renderer::ShaderUniformType::TEXTURE_MODE], (unsigned int)texture.textureMode());
	glUniform1uiv(_uniformLocation[Renderer::ShaderUniformType::TEXTURE_MAPS], (unsigned int)texture.nTextures(), texture.textureIds());
}


void Renderer::_applyTransform(const Entity& entity) const
{
	pushMatrix(MODEL);
	loadMatrix(MODEL, entity.transform().transformMatrix());
}


void Renderer::_renderMesh(const MeshComponent& mesh) const
{
	const MyMesh& meshData = mesh.mesh();

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