#ifndef __ENGINE_GAME_SCENE_H__
#define __ENGINE_GAME_SCENE_H__


#include <list>

#include "engine/game/gameObject.h"
#include "engine/game/components/camera.h"



class Scene
{

private:
	std::list<GameObject*> _gameObjects;	// all the objects of the world that are being rendered
	Camera *_activeCamera;					// the camera used to render the scene




public:
	Scene();
	virtual ~Scene();


public:
	void update();
	std::list<GameObject*> getObjectsByType(GameObject::Type type) const;


public:
	inline void addGameObject(GameObject* gameObject)	{ _gameObjects.push_back(gameObject);  }

	inline Camera* getActiveCamera() const				{ return _activeCamera; }
	inline void setActiveCamera(Camera *camera)			{ _activeCamera = camera; }


private:
	virtual void _init();					// concrete scenes must override this method to setup the game world

};


#endif // !__ENGINE_GAME_SCENE_H__