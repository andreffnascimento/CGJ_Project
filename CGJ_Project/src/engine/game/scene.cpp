#include "scene.h"

#include "engine/game/components/renderable.h"




Scene::Scene()
	: _gameObjects(std::list<GameObject*>()), _activeCamera(nullptr)
{
	//empty
}

Scene::~Scene()
{
	for (auto &gameObject : _gameObjects)
		delete gameObject;
}




void Scene::update()
{
	for (auto &gameObject : _gameObjects)
		gameObject->update();
}




template<class T>
std::list<T*> Scene::getObjectsByType() const
{
	T* temp = nullptr;
	std::list<T*> gameObjects = std::list<T*>();
	for (auto& object : _gameObjects)
		if ((temp = dynamic_cast<T*>(object)) != nullptr)
			gameObjects.push_back(temp);

	return gameObjects;
}

template std::list<Renderable*> Scene::getObjectsByType() const;