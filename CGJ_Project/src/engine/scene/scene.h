#ifndef __ENGINE_SCENE_SCENE_H__
#define __ENGINE_SCENE_SCENE_H__


#include <regex>
#include <string>
#include <unordered_set>

#include "engine/scene/ecsRegistry.h"
#include "engine/scene/entity.h"
#include "engine/scene/camera.h"
#include "engine/scene/components.h"




class Scene
{

private:
	ECSRegistry _registry = ECSRegistry();
	Camera _activeCamera = Camera();




protected:
	Scene() = default;
	Scene(const Scene& scene) = default;


public:
	virtual ~Scene() = default;


public:
	virtual void onCreate() = 0;
	void onUpdate();
	void onViewportResize(int width, int height);


public:
	Entity createEntity();
	Entity createEntity(const std::string& tag);

	void destroyEntity(EntityHandle entityHandler);

	std::unordered_set<Entity> getEntitiesByTag(const std::string& tag);
	std::unordered_set<Entity> getEntitiesByTag(const std::regex& regex);


public:
	inline void setActiveCamera(const Camera& camera) { _activeCamera = camera; }


public:		
	friend class Entity;

};


#endif // !__ENGINE_SCENE_SCENE_H__