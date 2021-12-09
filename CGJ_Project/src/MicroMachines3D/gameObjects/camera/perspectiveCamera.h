#ifndef __MM3D_GAMEOBJECTS_CAMERA_PERSPECTIVECAMERA_H__
#define __MM3D_GAMEOBJECTS_CAMERA_PERSPECTIVECAMERA_H__


#include "engine/math/AVTmathLib.h"
#include "engine/scene/components/camera.h"




class PerspectiveCamera : public Camera
{

private:
	float _fov;




public:
	PerspectiveCamera(float fov, float near, float far)
		: Camera(near, far), _fov(fov)
	{
		// empty
	}




public:
	void setViewport(int width, int height)
	{
		if (height == 0)					// prevent a divide by zero, when window is too small
			height = 1;

		glViewport(0, 0, width, height);	// set the viewport to be the entire window

		// set the projection matrix
		float ratio = (float)width / (float)height;
		loadIdentity(PROJECTION);
		perspective(_fov, ratio, _near, _far);
	}

};


#endif // !__MM3D_GAMEOBJECTS_CAMERA_PERSPECTIVECAMERA_H__