#ifndef __ENGINE_GAME_COMPONENTS_CAMERA_H__
#define __ENGINE_GAME_COMPONENTS_CAMERA_H__


#include "engine/game/gameObject.h"
#include "engine/utils/coords.h"


class Renderer;




class Camera : public GameObject
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
	void renderCamera(const Renderer &renderer) const;


public:
	virtual void setViewport(int width, int height) = 0;

};


#endif // !__ENGINE_GAME_COMPONENTS_CAMERA_H__
