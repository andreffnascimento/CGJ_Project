#include "engine/renderer/renderer.h"

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::renderMeshes(const Scene& scene) const
{
	const auto& meshComponents = scene.getSceneComponents<MeshComponent>();
	for (const auto& iterator : meshComponents)
	{
		const MeshComponent& mesh = iterator.second;
		if (!mesh.enabled())
			continue;

		const TransformComponent& transform = scene.getEntityById(iterator.first).transform();
		_loadMesh(mesh);
		_applyTransform(transform);
		_renderMesh(mesh);
	}
}




void Renderer::_loadMesh(const MeshComponent& mesh) const
{
	const MyMesh& meshData = mesh.meshData();
	GLint loc;
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, meshData.mat.ambient);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, meshData.mat.diffuse);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, meshData.mat.specular);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, meshData.mat.shininess);
}


void Renderer::_applyTransform(const TransformComponent& transform) const
{
	pushMatrix(MODEL);
	translate(MODEL, transform.translation().x, transform.translation().y, transform.translation().z);

	// avoid rotate operation if it is not needed (the other two are very common)
	if (transform.rotation().x != 0.0f)
		rotate(MODEL, transform.rotation().x, 1, 0, 0);

	if (transform.rotation().y != 0.0f)
		rotate(MODEL, transform.rotation().y, 0, 1, 0);

	if (transform.rotation().z != 0.0f)
		rotate(MODEL, transform.rotation().z, 0, 0, 1);

	scale(MODEL, transform.scale().x, transform.scale().y, transform.scale().z);
}


void Renderer::_renderMesh(const MeshComponent& mesh) const
{
	const MyMesh& meshData = mesh.meshData();

	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::VM], 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::PVM], 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(_uniformLocation[Renderer::ShaderUniformType::NORMAL], 1, GL_FALSE, mNormal3x3);

	glBindVertexArray(meshData.vao);	// render mesh

	if (!_shader.isProgramValid())
		throw std::string("Invalid shader program!");

	glDrawElements(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
}