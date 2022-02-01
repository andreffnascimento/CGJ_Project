#include "engine/renderer/renderer.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"




void Renderer::_renderShadowPlanes(const Scene& scene)
{
	for (auto& shadowPlaneIterator : scene.getSceneComponents<ShadowPlaneComponent>())
	{
		ShadowPlaneComponent& shadowPlane = shadowPlaneIterator.second;
		if (!shadowPlane.enabled() || !shadowPlane.mesh().enabled())
			continue;

		shadowPlane.mesh().setEnabled(false);
		_renderShadowPlane(scene, shadowPlane);
		shadowPlane.mesh().setEnabled(true);
	}
}




void Renderer::_enableShadowsRendering()
{
	glUniform1ui(_uniformLocator[RendererUniformLocations::RENDER_MODE], (GLuint)RendererSettings::RendererMode::SHADOWS_RENDERER);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
}


void Renderer::_disableShadowsRendering()
{
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
}


void Renderer::_renderShadowPlane(const Scene& scene, ShadowPlaneComponent& shadowPlane)
{
	_stencilShadowReflectionPlane(shadowPlane.mesh(), shadowPlane.transform());
	_enableShadowsRendering();

	for (const auto& lightIterator : scene.getSceneComponents<LightComponent>())
	{
		const LightComponent& light = lightIterator.second;
		if (!light.enabled() || !light.castShadows())
			continue;

		Coords3f translation;
		Entity lightEntity = scene.getEntityById(lightIterator.first);
		Transform::decomposeTransformMatrix(lightEntity, translation, Quaternion(), Coords3f());

		GLfloat floor_plane[4] = { 0,1,0,0 };
		float lightPos[4] = { translation.x, translation.y, translation.z, 1.0f };
		shadow_matrix(_modelTransforms.preModelTransform, floor_plane, lightPos);

		glCullFace(GL_FRONT);
		_renderModels(scene, RendererSettings::RendererMode::SHADOWS_RENDERER);
		_renderMeshes(scene, RendererSettings::RendererMode::SHADOWS_RENDERER);
		glCullFace(GL_BACK);

	}

	_modelTransforms.preModelTransform = TransformMatrix().setIdentityMatrix();
	_disableShadowsRendering();
}