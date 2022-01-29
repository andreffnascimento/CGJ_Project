#include "engine/renderer/renderer.h"

#include "engine/renderer/mesh/meshFromAssimp.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::_renderModels(const Scene& scene) const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)RendererSettings::RendererMode::MESH_RENDERER);

	for (auto& modelIterator : scene.getSceneComponents<ModelComponent>())
	{
		const ModelComponent& model = modelIterator.second;

		loadIdentity(MODEL);
		TransformMatrix modelBaseTransformMatrix = Transform::calculateTransformMatrix((TransformComponent&)model.modelTransform());
		TransformMatrix modelTransformMatrix = model.transform().transformMatrix() * modelBaseTransformMatrix;
		loadMatrix(MODEL, modelTransformMatrix);
		_recusiveModelRendering(model, model.scene()->mRootNode);
	}
}




void Renderer::_applyRecursiveModelTransform(const aiNode* node) const
{
	aiMatrix4x4 nodeTransform = node->mTransformation;
	nodeTransform.Transpose();

	pushMatrix(MODEL);
	multMatrix(MODEL, (float*)&nodeTransform);
}


void Renderer::_submitModelMeshData(const MyMesh& mesh) const
{
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_AMBIENT], 1, mesh.mat.ambient);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_DIFFUSE], 1, mesh.mat.diffuse);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_SPECULAR], 1, mesh.mat.specular);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_EMISSIVE], 1, mesh.mat.emissive);
	glUniform1f(_uniformLocator[RendererUniformLocations::MATERIAL_SHININESS], mesh.mat.shininess);

	glUniform1d(_uniformLocator[RendererUniformLocations::TEXTURE_MODE], 0);
}


void Renderer::_submitModelMeshInstance(const MyMesh& mesh) const
{
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	computeNormalMatrix3x3();

	glUniformMatrix4fv(_uniformLocator[RendererUniformLocations::INSTANCE_PVM_MATRIX], 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	glUniformMatrix4fv(_uniformLocator[RendererUniformLocations::INSTANCE_VM_MATRIX], 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix3fv(_uniformLocator[RendererUniformLocations::INSTANCE_NORMAL_MATRIX], 1, GL_FALSE, mNormal3x3);

	glBindVertexArray(mesh.vao);
	glDrawElements(mesh.type, mesh.numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void Renderer::_recusiveModelRendering(const ModelComponent& model, const aiNode* node) const
{
	_applyRecursiveModelTransform(node);

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		_submitModelMeshData(model.meshes()[node->mMeshes[i]]);
		_submitModelMeshInstance(model.meshes()[node->mMeshes[i]]);
	}

	for (unsigned int n = 0; n < node->mNumChildren; ++n)
		_recusiveModelRendering(model, node->mChildren[n]);

	popMatrix(MODEL);
}