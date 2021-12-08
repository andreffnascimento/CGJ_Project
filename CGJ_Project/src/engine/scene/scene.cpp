#include "scene.h"

#include "engine/scene/components/renderable.h"




Scene::Scene()
	: _entities(std::list<Entity*>()), _activeCamera(nullptr)
{
	//empty
}

Scene::~Scene()
{
	for (auto &gameObject : _entities)
		delete gameObject;
}




void Scene::update()
{
	for (auto &gameObject : _entities)
		gameObject->update();
}




template<class T>
std::list<T*> Scene::getEntitiesByType() const
{
	T* temp = nullptr;
	std::list<T*> entities = std::list<T*>();
	for (auto& object : _entities)
		if ((temp = dynamic_cast<T*>(object)) != nullptr)
			entities.push_back(temp);

	return entities;
}

template std::list<Renderable*> Scene::getEntitiesByType() const;