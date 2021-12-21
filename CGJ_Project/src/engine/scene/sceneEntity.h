#ifndef __engine_scene_sceneEntity__
#define __engine_scene_sceneEntity__


#include "engine/scene/entity.h"




class SceneEntity : public Entity
{

public:
	SceneEntity() = delete;
	SceneEntity(const SceneEntity&) = default;
	virtual ~SceneEntity() = default;


public:
	SceneEntity(const Entity& entity) : Entity(entity) {}

};


#endif // !__engine_scene_sceneEntity__
