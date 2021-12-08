#include "orthographicCamera.h"

#include "engine/math/AVTmathLib.h"




OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom, float near, float far)
	: Camera(near, far), _left(left), _right(right), _bottom(bottom), _top(top)
{
	// empty
}




void OrthographicCamera::setViewport(int width, int height)
{
	_top = _right * ((float)height / (float)width);
	_bottom = _left * ((float)height / (float)width);

	glViewport(0, 0, width, height);	// set the viewport to be the entire window

	// set the projection matrix
	loadIdentity(PROJECTION);
	ortho(_left, _right, _bottom, _top, _near, _far);
}