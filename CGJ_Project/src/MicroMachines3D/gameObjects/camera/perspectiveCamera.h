#ifndef __MM3D_GAMEOBJECTS_CAMERA_PERSPECTIVECAMERA_H__
#define __MM3D_GAMEOBJECTS_CAMERA_PERSPECTIVECAMERA_H__


#include "engine/math/AVTmathLib.h"
#include "engine/scene/components/camera.h"




class PerspectiveCamera : public Camera
{

private:
	float _fov;




public:
	PerspectiveCamera(float fov, float near, float far);


public:
	void setViewport(int width, int height) override;

};


#endif // !__MM3D_GAMEOBJECTS_CAMERA_PERSPECTIVECAMERA_H__