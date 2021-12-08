#include "scene.h"




Scene::Scene()
	: _gameObjects(std::list<GameObject*>()), _activeCamera(nullptr)
{
	_init();
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

std::list<GameObject*> Scene::getObjectsByType(GameObject::Type type) const
{
	std::list<GameObject*> gameObjects = std::list<GameObject*>();
	for (auto &gameObject : _gameObjects)
		if (gameObject->getType() == type)
			gameObjects.push_back(gameObject);

	return gameObjects;
}




void Scene::_init()
{
	// empty
}