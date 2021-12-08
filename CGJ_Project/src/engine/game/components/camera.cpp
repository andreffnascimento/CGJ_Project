#include "camera.h"

#include "engine/math/AVTmathLib.h"




Camera::Camera()
	: _cameraCoords({ 0.0f, 0.0f, 0.0f }), _targetCoords({ 0.0f, 0.0f, 0.0f }), _near(-1.0f), _far(1.0f)
{
	// emtpy
}

Camera::Camera(float near, float far)
	: _cameraCoords({ 0.0f, 0.0f, 0.0f }), _targetCoords({ 0.0f, 0.0f, 0.0f }), _near(near), _far(far)
{
	// emtpy
}




void Camera::setPosition(float x, float y, float z)
{
	_cameraCoords.x = x;
	_cameraCoords.y = y;
	_cameraCoords.z = z;
}

void Camera::setTarget(float x, float y, float z)
{
	_targetCoords.x = x;
	_targetCoords.y = y;
	_targetCoords.z = z;
}




void Camera::render()
{
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(_cameraCoords.x, _cameraCoords.y, _cameraCoords.z,	// camera position
		   _targetCoords.x, _targetCoords.y, _targetCoords.z,	// target position
		   0, 1, 0);											// up vector
}