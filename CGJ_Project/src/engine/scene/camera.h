#ifndef __ENGINE_SCENE_CAMERA_H__
#define __ENGINE_SCENE_CAMERA_H__


#include "engine/scene/entity.h"
#include "engine/scene/components.h"




class Camera : public Entity
{

private:
	CameraComponent* _cameraComponent = nullptr;




public:
	Camera() = default;
	Camera(const Camera&) = default;
	virtual ~Camera() = default;


public:
	Camera(const Entity& entity)
		: Entity(entity), _cameraComponent(&entity.getComponent<CameraComponent>()) {}


public:
	inline operator CameraComponent& ()	{ _validateCamera();  return *_cameraComponent; }
	inline bool isCameraValid() const	{ return _cameraComponent != nullptr;  }


private:
	void _validateCamera() const
	{
		if (!isCameraValid())
			throw std::string("Unable to use a camara that wasn't initialized");
	}

};


#endif // !__ENGINE_SCENE_CAMERA_H__