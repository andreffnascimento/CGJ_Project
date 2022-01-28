#include "engine/renderer/renderer.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"



void Renderer::_renderPlanarReflections(const Scene& scene) const
{
	Entity table = scene.getEntityByTag("Table:top"); 

	// How to render reflections:
	// 1. Render "Table:top" into stencil buffer, without sending it to the color buffer
	// 2. Render the flipped meshes into the stenciled area
	// 3. Enable blending, render the "top" into the color buffer so that the reflections blend with it
	// 4. Disable blending, render all the meshes normally

	const MeshData* tableMesh = &table.getComponent<MeshComponent>().meshData();

	// -- Stencil plane shape ------------------------------------------------- //
	_enableReflectorPlaneStenciling();

	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();

	_submitMeshData(*tableMesh);

	_addObjectToInstanceBuffer(instanceBuffer, &table.transform());
	_submitRenderableData(*tableMesh, instanceBuffer);


	// -- Draw scene into stenciled area -------------------------------------- //
	_enableReflectionsRendering();

	//TODO: scale all meshes by (1, -1, 1)

	_renderLights(scene);  // light position needs to be flipped -> lightPos[1] *= (-1.0f)
	_renderMeshes(scene);  // should render meshes excluding the table mesh -> a clipping plane could be used to not render anything below table top
	_renderModels(scene);

	_disableReflectionsRendering();

	// -- Blend the table color with the reflected meshes  ------------------- //
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_submitRenderableData(*tableMesh, instanceBuffer);

	glDisable(GL_BLEND);

}


void Renderer::_enableReflectorPlaneStenciling() const
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NEVER, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
}


void Renderer::_enableReflectionsRendering() const
{
	// Draw the reflected objects where stencil is 1
	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//pushMatrix(MODEL);
	//scale(MODEL, 1.0f, -1.0f, 1.0f);
	glCullFace(GL_FRONT);
}


void Renderer::_disableReflectionsRendering() const
{
	//popMatrix(MODEL);
	glCullFace(GL_BACK);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glClear(GL_STENCIL_BUFFER_BIT);
}
