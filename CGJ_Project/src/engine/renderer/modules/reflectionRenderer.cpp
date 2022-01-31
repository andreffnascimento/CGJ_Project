#include "engine/renderer/renderer.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"



void Renderer::_renderPlanarReflections(const Scene& scene)
{
	_stencilReflectorPlane(scene);
	_enableReflectionsRendering();

	// TODO flip position of all lightsources on y axis -> lightPos.y *= -1.0f

	_renderMeshes(scene);
	_renderModels(scene);

	_disableReflectionsRendering();
	_blendReflections(scene);
}

void Renderer::_stencilReflectorPlane(const Scene& scene)
{
	Entity table = scene.getEntityByTag("Table:top");

	const MeshData* tableMesh = &table.getComponent<MeshComponent>().meshData();
	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();

	_enableReflectorPlaneStenciling();

	_submitMeshData(*tableMesh);
	_addObjectToInstanceBuffer(instanceBuffer, &table.transform());
	_submitRenderableData(*tableMesh, instanceBuffer);
}


void Renderer::_blendReflections(const Scene& scene)
{
	Entity table = scene.getEntityByTag("Table:top");

	const MeshData* tableMesh = &table.getComponent<MeshComponent>().meshData();
	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_submitMeshData(*tableMesh);
	_addObjectToInstanceBuffer(instanceBuffer, &table.transform());
	_submitRenderableData(*tableMesh, instanceBuffer);

	glDisable(GL_BLEND);
}


void Renderer::_enableReflectorPlaneStenciling()
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NEVER, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
}


void Renderer::_enableReflectionsRendering()
{
	//_modelTransforms.applyPostMatrix = true;
	//_modelTransforms.postModelTransform = TransformMatrix().setScaleMatrix({ 1.0f, -1.0f, 1.0f });
	_modelTransforms.preModelTransform = TransformMatrix().setScaleMatrix({ 1.0f, -1.0f, 1.0f });

	// Draw the reflected objects where stencil is 1
	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glCullFace(GL_FRONT);
}


void Renderer::_disableReflectionsRendering()
{
	//_modelTransforms.applyPostMatrix = false;
	_modelTransforms.preModelTransform = TransformMatrix().setIdentityMatrix();

	glCullFace(GL_BACK);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glClear(GL_STENCIL_BUFFER_BIT);
}
