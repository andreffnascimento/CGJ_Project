#include "engine/renderer/renderer.h"

#include "engine/renderer/mesh/meshFromAssimp.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




void Renderer::_renderModels(const Scene& scene, RendererSettings::RendererMode renderMode) const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (unsigned int)renderMode);

	for (auto& modelIterator : scene.getSceneComponents<ModelComponent>())
	{
		const ModelComponent& model = modelIterator.second;
		TransformMatrix modelBaseTransformMatrix = Transform::calculateTransformMatrix((TransformComponent&)model.modelTransform());
		const TransformMatrix& modelTransform = scene.getEntityById(modelIterator.first).transform().transformMatrix();

		loadMatrix(MODEL, _modelTransforms.preModelTransform);
		multMatrix(MODEL, modelTransform * modelBaseTransformMatrix);
		if (_modelTransforms.applyPostMatrix)
			multMatrix(MODEL, _modelTransforms.postModelTransform);

		_recusiveModelRendering(model);
	}
}




void Renderer::_submitModelMeshData(const MyMesh& mesh) const
{
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_AMBIENT], 1, mesh.mat.ambient);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_DIFFUSE], 1, mesh.mat.diffuse);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_SPECULAR], 1, mesh.mat.specular);
	glUniform4fv(_uniformLocator[RendererUniformLocations::MATERIAL_EMISSIVE], 1, mesh.mat.emissive);
	glUniform1f(_uniformLocator[RendererUniformLocations::MATERIAL_SHININESS], mesh.mat.shininess);

	glUniform1ui(_uniformLocator[RendererUniformLocations::TEXTURE_MODE], (unsigned int)Texture::TextureMode::NONE);
	glUniform1ui(_uniformLocator[RendererUniformLocations::TEXTURE_N_COLOR_MAPS], 0);
	glUniform1ui(_uniformLocator[RendererUniformLocations::TEXTURE_N_NORMAL_MAPS], 0);
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


void Renderer::_recusiveModelRendering(const ModelComponent& model) const
{
	for (unsigned int i = 0; i < model.meshes().size(); i++)
	{
		_submitModelMeshData(model.meshes()[i]);
		_submitModelMeshInstance(model.meshes()[i]);
	}
}