#include "perspectiveCamera.h"

#include "engine/math/AVTmathLib.h"




PerspectiveCamera::PerspectiveCamera(float fov, float near, float far)
	: Camera(near, far), _fov(fov)
{
	// empty
}




void PerspectiveCamera::setViewport(int width, int height)
{
	if (height == 0)					// prevent a divide by zero, when window is too small
		height == 1;

	glViewport(0, 0, width, height);	// set the viewport to be the entire window

	// set the projection matrix
	float ratio = (float)width / (float)height;
	loadIdentity(PROJECTION);
	perspective(_fov, ratio, _near, _far);
}
