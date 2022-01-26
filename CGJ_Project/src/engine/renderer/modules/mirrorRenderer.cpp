#include "engine/renderer/renderer.h"

#include "engine/math/AVTmathLib.h"
#include "engine/math/transform.h"



void Renderer::_renderMirror(const Scene& scene) const
{
	Entity mirrorEntity = scene.getEntityByTag("RearViewMirror");

	const MeshComponent& mirrorMesh = mirrorEntity.getComponent<MeshComponent>();


	// -- Stencil mirror shape ------------------------------------------------- //
	_enableShapeStenciling();

	RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();
	const MeshData* meshData = &mirrorMesh.meshData();

	_submitMeshData(*meshData);

	const TransformComponent* transform = &mirrorEntity.transform();

	_addObjectToInstanceBuffer(instanceBuffer, transform);
	_submitRenderableData(*meshData, instanceBuffer);


	// -- Setup the mirror camera ---------------------------------------------- //
	const Coords3f& cameraCoords = mirrorEntity.transform().translation();
	const Coords3f& targetCoords = scene.getEntityByTag("Car").getComponent<RigidbodyComponent>().position();  // shortcut for now


	Coords3f up = { 0.0f, 1.0f, 0.0f };
	if (cameraCoords.x == 0.0f && cameraCoords.y != 0.0f && cameraCoords.z == 0.0f)
		up = { 0.0f, 0.0f, -1.0f };


	pushMatrix(MODEL);
	loadIdentity(MODEL);
	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	pushMatrix(VIEW);
	loadIdentity(VIEW);

	perspective(70, 1.2f, 0.1f, 1000.0f); // TODO correct aspect ratio based on viewport width and height

	lookAt(cameraCoords.x, cameraCoords.y, cameraCoords.z,	// camera position
		targetCoords.x, targetCoords.y + 4.0f, targetCoords.z,	// target position
		up.x, up.y, up.z);


	// -- Draw scene into stenciled area -------------------------------------- //
	_enableRenderingIntoStencil();

	Renderer::_renderMeshes(scene);

	popMatrix(PROJECTION);
	popMatrix(VIEW);
	popMatrix(MODEL);

	_disableStencilRendering();
	
}

void Renderer::_renderMirrors(const Scene& scene) const
{
	//const auto& mirrorComponents = scene.getSceneComponents<MirrorComponent>();

	//for (const auto & mirrorIterator : mirrorComponents)
	//{
	//	const MirrorComponent& mirror = mirrorIterator.first;
	//	const TransformComponent* transform = mirrorIterator.second;

	//	_stencilMesh(mirror.mesh(), transform);

	//	const Coords3f& cameraCoords = mirror.rigidbody()->position(); 
	//	//const Coords3f& targetCoords = cameraCoords + mirror.mirrorNormal();  // TODO rotate normal by mesh rotation
	//	const Coords3f& targetCoords = scene.getEntityByTag("Car").getComponent<RigidbodyComponent>().position();  // shortcut for now


	//	Coords3f up = { 0.0f, 1.0f, 0.0f };
	//	if (cameraCoords.x == 0.0f && cameraCoords.y != 0.0f && cameraCoords.z == 0.0f)
	//		up = { 0.0f, 0.0f, -1.0f };


	//	loadIdentity(VIEW);
	//	loadIdentity(MODEL);
	//	lookAt(cameraCoords.x, cameraCoords.y, cameraCoords.z,	// camera position
	//		targetCoords.x, targetCoords.y, targetCoords.z,	// target position
	//		up.x, up.y, up.z);								// up vector

	//	Renderer::renderMeshes(scene);


	//}

	//_disableStencilRendering();
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


void Renderer::_stencilMesh(MeshComponent& mesh, TransformComponent& transform) const
{
	//_enableShapeStenciling();

	//RendererData::SubmitInstanceBuffer instanceBuffer = RendererData::SubmitInstanceBuffer();

	//_submitMeshData(mesh.meshData());
	//_addToInstanceBuffer(instanceBuffer, transform);

}