#include "engine/renderer/renderer.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"



void Renderer::_renderShadows(const Scene& scene)
{
	Entity table = scene.getEntityByTag("Table:top");
	const MeshData* tableMesh = &table.getComponent<MeshComponent>().meshData();
	

	// -- Stencil plane shape ------------------------------------------------- //
	_enableReflectorPlaneStenciling();

	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();

	_submitMeshData(*tableMesh);

	_addObjectToInstanceBuffer(instanceBuffer, &table.transform());
	_submitRenderableData(*tableMesh, instanceBuffer);


	// -- Draw scene into stenciled area -------------------------------------- //
	_enableShadowsRendering();

	GLfloat floor_plane[4] = { 0,1,0,0 };


	const auto& lightComponents = scene.getSceneComponents<LightComponent>();
	if (lightComponents.size() > RendererSettings::MAX_LIGHTS)
		throw std::string("The renderer is not able to process all the game lights");

	for (const auto& lightIterator : lightComponents)
	{
		Entity lightEntity = scene.getEntityById(lightIterator.first);
		const LightComponent& light = lightIterator.second;

		if (!light.isEnabled())
			continue;

		Coords3f translation;
		Quaternion rotation;
		Coords3f scale;
		Transform::decomposeTransformMatrix(lightEntity, translation, rotation, scale);

		if (light.lightType() == LightComponent::LightType::POINT)
		{
			float lightPos[4] = { translation.x, translation.y, translation.z, 0.0f };
			float mat[16];
			shadow_matrix(mat, floor_plane, lightPos);

			_modelTransforms.preModelTransform = TransformMatrix(mat);

			_renderModels(scene, RendererSettings::RendererMode::SHADOWS_RENDERER);
			_renderMeshes(scene, RendererSettings::RendererMode::SHADOWS_RENDERER);

			_modelTransforms.preModelTransform = TransformMatrix().setIdentityMatrix();


			glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (GLuint)RendererSettings::RendererMode::MESH_RENDERER);

			_submitMeshData(*tableMesh);

			_addObjectToInstanceBuffer(instanceBuffer, &table.transform());
			_submitRenderableData(*tableMesh, instanceBuffer);

		}
	}

	_modelTransforms.preModelTransform = TransformMatrix().setIdentityMatrix();

	_disableShadowsRendering();
}


void Renderer::_enableShadowsRendering() const
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (GLuint)RendererSettings::RendererMode::SHADOWS_RENDERER);

	glDisable(GL_DEPTH_TEST); //To force the shadow geometry to be rendered even if behind the floor

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
}


void Renderer::_disableShadowsRendering() const
{
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
