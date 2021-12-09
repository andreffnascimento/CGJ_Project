#include "scene.h"

#include "engine/scene/components/renderable.h"




Scene::Scene()
	: _registry(std::list<Entity*>()), _activeCamera(nullptr)
{
	//empty
}

Scene::~Scene()
{
	for (auto &gameObject : _registry)
		delete gameObject;
}




void Scene::update()
{
	for (auto &gameObject : _registry)
		gameObject->update();
}


void Scene::addEntity(Entity* entity)
{
	for (auto &child : entity->_children)
		addEntity(child);

	if (entity->_scene != nullptr)			// entities can only be associated once to a scene
		return;

	entity->_scene = this;
	_registry.push_back(entity);
}




#include <iostream>
template<class T>
std::list<T*> Scene::getEntitiesByType() const
{
	T* temp = nullptr;
	std::list<T*> entities = std::list<T*>();
	for (auto& object : _registry)
		if ((temp = dynamic_cast<T*>(object)) != nullptr)
			entities.push_back(temp);

	return entities;
}

template std::list<Camera*> Scene::getEntitiesByType() const;
template std::list<Renderable*> Scene::getEntitiesByType() const;