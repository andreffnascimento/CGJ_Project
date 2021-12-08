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
	OrthographicCamera(float left, float right, float bottom, float top, float near, float far);


public:
	void setViewport(int width, int height) override;

};


#endif // !__MM3D_GAMEOBJECTS_CAMERA_ORTHOGRAPHICCAMERA_H__