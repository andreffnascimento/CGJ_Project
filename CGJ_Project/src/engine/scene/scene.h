#ifndef __ENGINE_SCENE_SCENE_H__
#define __ENGINE_SCENE_SCENE_H__


#include <list>

#include "engine/scene/entity.h"
#include "engine/scene/components/camera.h"



class Scene
{

private:
	std::list<Entity*> _registry;			// all the entities of scene
	Camera *_activeCamera;					// the camera used to render the scene




public:
	Scene();
	virtual ~Scene();


public:
	void update();

	void addEntity(Entity* entity);


public:
	template<class T>
	std::list<T*> getEntitiesByType() const;


public:
	inline Camera* getActiveCamera() const				{ return _activeCamera; }
	inline void setActiveCamera(Camera *camera)			{ _activeCamera = camera; }


private:
	virtual void _init() = 0;				// concrete scenes must override this method to setup the game world

};


#endif // !__ENGINE_SCENE_SCENE_H__