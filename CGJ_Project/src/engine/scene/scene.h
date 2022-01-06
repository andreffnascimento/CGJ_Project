#ifndef __engine_scene_scene__
#define __engine_scene_scene__


#include <regex>
#include <string>
#include <unordered_set>

#include "engine/scene/ecsRegistry.h"
#include "engine/scene/entity.h"
#include "engine/scene/cameraEntity.h"



class Scene
{

private:
	ECSRegistry _registry = ECSRegistry();
	CameraEntity _activeCamera = CameraEntity();

	float _timeAccumulator = 0.0f;




protected:
	Scene() = default;
	Scene(const Scene& scene) = default;


public:
	virtual ~Scene() = default;


public:
	void onCreate();
	void onUpdate(float ts);
	void onFixedUpdate(float ts);
	void onViewportResize(int width, int height);


public:
	virtual void setupScene() = 0;


public:
	Entity createEntity();
	Entity createEntity(const std::string& tag, bool joinId = false);

	bool hasEntityWithTag(const std::string& tag) const;

	Entity getEntityById(const EntityHandle& entityHandle) const;
	Entity getEntityByTag(const std::string& tag) const;
	std::unordered_set<Entity> getEntitiesByTag(const std::regex& regex) const;

	template <typename T>
	std::unordered_map<EntityHandle, T>& getSceneComponents() const
	{
		return _registry.getComponents<T>();
	}


public:
	inline const CameraEntity& activeCamera() const						{ return _activeCamera; }
	void setActiveCamera(const CameraEntity& camera);




public:		
	friend class Entity;

};


#endif // !__engine_scene_scene__