#include "engine/renderer/renderer.h"




void Renderer::renderCamera(const Scene& scene) const
{
	const CameraEntity& camera = scene.activeCamera();
	TransformComponent& transform = camera.getComponent<TransformComponent>();
	CameraComponent& cameraSettings = camera.getComponent<CameraComponent>();

	const Coords3f& cameraCoords = transform.translation();
	const Coords3f& targetCoords = cameraSettings.targetCoords();

	Coords3f up = { 0.0f, 1.0f, 0.0f };
	if (cameraCoords.x == 0.0f && cameraCoords.y != 0.0f && cameraCoords.z == 0.0f)
		up = { 0.0f, 0.0f, -1.0f };

	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(cameraCoords.x, cameraCoords.y, cameraCoords.z,	// camera position
		targetCoords.x, targetCoords.y, targetCoords.z,	// target position
		up.x, up.y, up.z);								// up vector
}




void Renderer::_setOrthographicViewport(CameraComponent& camera, int width, int height) const
{
	camera.setOrthographicCamera(camera.clippingPlanes(), camera.viewportRect().right);

	float left = camera.viewportRect().left;
	float right = camera.viewportRect().right;
	float bottom = camera.viewportRect().bottom;
	float top = camera.viewportRect().top;
	float near = camera.clippingPlanes().near;
	float far = camera.clippingPlanes().far;

	glViewport(0, 0, width, height);	// set the viewport to be the entire window

	// set the projection matrix
	loadIdentity(PROJECTION);
	ortho(left, right, bottom, top, near, far);
}


void Renderer::_setPerspectiveViewport(CameraComponent& camera, int width, int height) const
{
	if (height == 0)					// prevent a divide by zero, when window is too small
		height = 1;

	glViewport(0, 0, width, height);	// set the viewport to be the entire window

	// set the projection matrix
	float ratio = (float)width / (float)height;
	loadIdentity(PROJECTION);
	perspective(camera.fov(), ratio, camera.clippingPlanes().near, camera.clippingPlanes().far);
}