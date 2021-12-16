#ifndef __ENGINE_SCENE_CAMERA_H__
#define __ENGINE_SCENE_CAMERA_H__


#include "engine/scene/entity.h"
#include "engine/scene/components.h"




class Camera : public Entity
{

private:
	CameraComponent* _cameraComponent = nullptr;




private:
	Camera() = default;


public:
	Camera(const Camera&) = default;
	virtual ~Camera() = default;


public:
	Camera(const Entity& entity)
		: Entity(), _cameraComponent(&entity.getComponent<CameraComponent>()) {}


public:
	inline operator CameraComponent& ()				{ _validateCamera();  return *_cameraComponent; }


private:
	void _validateCamera() const
	{
		if (_cameraComponent == nullptr)
			throw std::string("Unable to use a camara that wasn't initialized");
	}


public:
	friend class Scene;

};


#endif // !__ENGINE_SCENE_CAMERA_H__
