#include "engine/renderer/renderer.h"

#include "engine/math/AVTmathLib.h"




void Renderer::_renderPlanarReflections(const Scene& scene)
{
	for (auto& reflectionPlaneIterator : scene.getSceneComponents<ReflectionPlaneComponent>())
	{
		ReflectionPlaneComponent& reflectionPlane = reflectionPlaneIterator.second;
		if (!reflectionPlane.enabled() || !reflectionPlane.mesh().enabled())
			continue;

		reflectionPlane.mesh().setEnabled(false);
		_renderReflectionPlane(scene, reflectionPlane);
		reflectionPlane.mesh().setEnabled(true);
	}
}




void Renderer::_initShadowReflectionPlaneStenciling() const
{
	glClearStencil(0x0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NEVER, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
}


void Renderer::_terminateShadowReflectionPlaneStenciling() const
{
	glDisable(GL_STENCIL_TEST);
}


void Renderer::_stencilShadowReflectionPlane(const MeshComponent& mesh, const TransformComponent& transform) const
{
	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	_initShadowReflectionPlaneStenciling();
	_submitMeshData(mesh.meshData());
	_addObjectToInstanceBuffer(instanceBuffer, &transform);
	_submitRenderableData(mesh.meshData(), instanceBuffer);
	_terminateShadowReflectionPlaneStenciling();
}




void Renderer::_enableReflectionsRendering()
{
	glEnable(GL_STENCIL_TEST);
	glCullFace(GL_FRONT);
	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	_modelTransforms.preModelTransform = TransformMatrix().setScaleMatrix({ 1.0f, -1.0f, 1.0f });
}


void Renderer::_disableReflectionsRendering()
{
	_modelTransforms.preModelTransform = TransformMatrix().setIdentityMatrix();

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
}


void Renderer::_renderReflectionPlane(const Scene& scene, ReflectionPlaneComponent& reflectionPlane)
{
	_stencilShadowReflectionPlane(reflectionPlane.mesh(), reflectionPlane.transform());
	_enableReflectionsRendering();

	// TODO flip position of all lightsources on y axis -> lightPos.y *= -1.0f
	_renderModels(scene, RendererSettings::RendererMode::MESH_RENDERER);
	_renderMeshes(scene, RendererSettings::RendererMode::MESH_RENDERER);

	_disableReflectionsRendering();
}