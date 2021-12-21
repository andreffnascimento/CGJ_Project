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
	void onCreate();
	void onUpdate(float ts);
	void onViewportResize(int width, int height);


public:
	virtual void setupScene() = 0;


public:
	Entity createEntity();
	Entity createEntity(const std::string& tag, bool joinId = false);

	void destroyEntity(EntityHandle entityHandler);

	bool hasEntityWithTag(const std::string& tag) const;

	Entity getEntityByTag(const std::string& tag);
	std::unordered_set<Entity> getEntitiesByTag(const std::regex& regex);


public:
	void setActiveCamera(const Camera& camera);


public:		
	friend class Entity;

};


#endif // !__ENGINE_SCENE_SCENE_H__