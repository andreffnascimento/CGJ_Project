#include "engine/renderer/renderer.h"

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];


void Renderer::_renderFixedMirrors(const Scene& scene) const
{

}


void Renderer::_renderFixedMirror(const Scene& scene) const
{
	/* ------------------ INIT ------------------ */

	// Set projection to orthographic so that the mirror is always in the same position on the screen
	WindowCoords windowSize = Application::getInstance().getWindowSize();

	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	pushMatrix(VIEW);
	loadIdentity(VIEW);
	ortho(0, (float)windowSize.x, 0, (float)windowSize.y, -1.0f, 1.0f);

	/* INIT END -------------------------------- */



	/* -------- Shortcut -------- */
	Entity mirrorEntity = scene.getEntityByTag("RearViewMirror");  

	const FixedMirrorComponent& fixedMirrorComponent = mirrorEntity.getComponent<FixedMirrorComponent>();
	const MeshData* meshData = &fixedMirrorComponent.meshData();
	const TransformComponent* transform = &mirrorEntity.transform();

	// -- Stencil mirror shape ------------------------------------------------- //
	_enableShapeStenciling();

	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();

	_submitMeshData(*meshData);

	_addObjectToInstanceBuffer(instanceBuffer, transform);

	_submitRenderableData(*meshData, instanceBuffer);  // Stencil the mirror shape in orthographic projection

	// Pop the matrices used for the orthographic effect
	popMatrix(PROJECTION);
	popMatrix(VIEW);


	// -- Setup the mirror camera ---------------------------------------------- //
	const Coords3f& targetCoords = fixedMirrorComponent.lookAt();
	const Coords3f& cameraCoords = fixedMirrorComponent.cameraPosition();

	Coords3f up = { 0.0f, 1.0f, 0.0f };

	// Push new matrices onto the stack, since the scene will be rendered again
	pushMatrix(MODEL);
	loadIdentity(MODEL);
	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	pushMatrix(VIEW);
	loadIdentity(VIEW);

	float aspectRatio = windowSize.x / windowSize.y;
	// This doesn't work correctly, however it would work if we weren't doing the orhto() shenanigans in order to 
	// always show the mirror in the same place on the screen. It has something to do with the matrices if I had to guess.
	perspective(70.0f, aspectRatio, 0.1f, 1000.0f);

	lookAt(cameraCoords.x, cameraCoords.y, cameraCoords.z,	// camera position
		targetCoords.x, targetCoords.y, targetCoords.z,	// target position
		up.x, up.y, up.z);


	// -- Draw scene into stenciled area -------------------------------------- //
	_enableRenderingIntoStencil();

	// What's rendered here will appear in the mirror
	_renderSkybox();
	_renderMeshes(scene);
	_renderParticles(scene);

	popMatrix(PROJECTION);
	popMatrix(VIEW);
	popMatrix(MODEL);

	_disableStencilRendering();
}


void Renderer::_enableShapeStenciling() const
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 0x1, 0x0); // Stenciled area = 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}


void Renderer::_enableRenderingIntoStencil() const
{
	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Don't change stencil buffer
}


void Renderer::_disableStencilRendering() const
{
	glDisable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
}
