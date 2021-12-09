#ifndef __ENGINE_SCENE_COMPONENTS_CAMERA_H__
#define __ENGINE_SCENE_COMPONENTS_CAMERA_H__


#include "engine/scene/entity.h"
#include "engine/utils/coords.h"


class Renderer;




class Camera : public Entity
{

protected:
	Coords3f _cameraCoords;		// camera position in the world
	Coords3f _targetCoords;		// target position in the world
	float _near;				// camera near plane
	float _far;					// camera far plane




public:
	Camera();
	Camera(float near, float far);


public:
	void setPosition(float x, float y, float z);
	void setTarget(float x, float y, float z);


public:
	inline void setNear(float near) { _near = near; }
	inline void setFar(float far)	{ _far = far; }


public:
	void renderCamera(const Renderer &renderer) const;


public:
	virtual void setViewport(int width, int height) = 0;

};


#endif // !__ENGINE_SCENE_COMPONENTS_CAMERA_H__
