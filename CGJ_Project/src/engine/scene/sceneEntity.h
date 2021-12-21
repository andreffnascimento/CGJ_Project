#ifndef __ENGINE_SCENE_SCENEENTITY_H__
#define __ENGINE_SCENE_SCENEENTITY_H__


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


#endif // !__ENGINE_SCENE_SCENEENTITY_H__
