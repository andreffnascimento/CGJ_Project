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
	unsigned int nMeshes = 1;
	float shininess[] = { material.shininess };
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_AMBIENT], nMeshes, material.ambient);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_DIFFUSE], nMeshes, material.diffuse);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_SPECULAR], nMeshes, material.specular);
	glUniform1fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_SHININESS], nMeshes, shininess);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::MATERIAL_EMISSIVE], nMeshes, material.emissive);
}


void Renderer::_loadTexture(const MeshComponent& mesh) const
{
	const MeshComponent::Texture& texture = mesh.texture();
	unsigned int nMeshes = 1;
	unsigned int nTextures[] = { (unsigned int)texture.nTextures() };
	unsigned int textureMode[] = { (unsigned int)texture.textureMode() };

	glUniform1uiv(_uniformLocation[RendererData::ShaderUniformType::N_TEXTURES], nMeshes, nTextures);
	glUniform1uiv(_uniformLocation[RendererData::ShaderUniformType::TEXTURE_MODE], nMeshes, textureMode);
	glUniform1uiv(_uniformLocation[RendererData::ShaderUniformType::TEXTURE_MAPS], nMeshes * (unsigned int)texture.nTextures(), texture.textureIds());
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
	computeNormalMatrix3x3();
	glUniformMatrix4fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_VM_MATRIX], 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_PVM_MATRIX], 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	glUniformMatrix3fv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_NORMAL_MATRIX], 1, GL_FALSE, mNormal3x3);
	glUniform1uiv(_uniformLocation[RendererData::ShaderUniformType::INSTANCE_MESH_INDEX], 1, 0);

	glBindVertexArray(meshData.vao);

	if (!_shader.isProgramValid())
		throw std::string("Invalid shader program!");

	unsigned int nInstances = 1;
	glDrawElementsInstanced(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0, nInstances);
	glBindVertexArray(0);
	popMatrix(MODEL);
}