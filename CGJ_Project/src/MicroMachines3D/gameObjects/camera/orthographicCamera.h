#ifndef __MM3D_GAMEOBJECTS_CAMERA_ORTHOGRAPHICCAMERA_H__
#define __MM3D_GAMEOBJECTS_CAMERA_ORTHOGRAPHICCAMERA_H__


#include "engine/math/AVTmathLib.h"
#include "engine/scene/components/camera.h"




class OrthographicCamera : public Camera
{

private:
	float _left;
	float _right;
	float _bottom;
	float _top;




public:
	OrthographicCamera(float left, float right, float top, float bottom, float near, float far)
		: Camera(near, far), _left(left), _right(right), _bottom(bottom), _top(top)
	{
		// empty
	}




public:
	void setViewport(int width, int height)
	{
		_top = _right * ((float)height / (float)width);
		_bottom = _left * ((float)height / (float)width);

		glViewport(0, 0, width, height);	// set the viewport to be the entire window

		// set the projection matrix
		loadIdentity(PROJECTION);
		ortho(_left, _right, _bottom, _top, _near, _far);
	}

};


#endif // !__MM3D_GAMEOBJECTS_CAMERA_ORTHOGRAPHICCAMERA_H__