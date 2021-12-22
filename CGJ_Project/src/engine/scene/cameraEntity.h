#ifndef __engine_scene_cameraEntity__
#define __engine_scene_cameraEntity__


#include "engine/scene/entity.h"
#include "engine/scene/components.h"




class CameraEntity : public Entity
{

private:
	CameraComponent* _cameraComponent = nullptr;




public:
	CameraEntity() = default;
	CameraEntity(const CameraEntity&) = default;
	virtual ~CameraEntity() = default;


public:
	CameraEntity(const Entity& entity)
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


#endif // !__engine_scene_cameraEntity__