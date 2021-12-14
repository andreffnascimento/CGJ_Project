#ifndef __ENGINE_SCENE_SCENE_H__
#define __ENGINE_SCENE_SCENE_H__


#include <regex>
#include <string>
#include <unordered_set>

#include "engine/scene/ecsRegistry.h"


class Entity;




class Scene
{

private:
	ECSRegistry _registry = ECSRegistry();
	//Camera* _activeCamera;




protected:
	Scene() = default;
	Scene(const Scene& scene) = delete;
	virtual ~Scene() = default;


protected:
	Entity createEntity();
	Entity createEntity(const std::string& tag);

	void destroyEntity(EntityHandle entityHandler);

	std::unordered_set<Entity> getEntitiesByTag(const std::string& tag);
	std::unordered_set<Entity> getEntitiesByTag(const std::regex& regex);


public:
	virtual void onCreate() = 0;
	void onUpdate();


public:
	friend class Entity;

};


#endif // !__ENGINE_SCENE_SCENE_H__