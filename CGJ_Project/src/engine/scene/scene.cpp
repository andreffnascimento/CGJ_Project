#include "scene.h"

#include <unordered_map>

#include "engine/app/application.h"
#include "engine/renderer/renderer.h"




Entity Scene::createEntity()
{
	Entity entity = Entity(_registry.createEntity(), this);
	entity.addComponent<TagComponent>("Entity$" + (std::string)entity);
	entity.addComponent<TransformComponent>();
	return entity;
}

Entity Scene::createEntity(const std::string& tag)
{
	Entity entity = Entity(_registry.createEntity(), this);
	entity.addComponent<TagComponent>(tag);
	entity.addComponent<TransformComponent>();
	return entity;
}



void Scene::destroyEntity(EntityHandle entityHandle)
{
	_registry.destroyEntity(entityHandle);
}




std::unordered_set<Entity> Scene::getEntitiesByTag(const std::string& tag)
{
	auto& taggedEntities = _registry.getComponents<TagComponent>();
	auto entities = std::unordered_set<Entity>();
	for (auto& iterator : taggedEntities)
		if (iterator.second == tag)
			entities.emplace(iterator.first, this);
	return entities;
}

std::unordered_set<Entity> Scene::getEntitiesByTag(const std::regex& regex)
{
	auto& taggedEntities = _registry.getComponents<TagComponent>();
	auto entities = std::unordered_set<Entity>();
	for (auto& iterator : taggedEntities)
		if (iterator.second == regex)
			entities.emplace(Entity(iterator.first, this));
	return entities;
}




void Scene::onUpdate()
{
	Renderer& renderer = Application::getRenderer();
	renderer.initSceneRendering();
}


void Scene::onViewportResize(int width, int height)
{
	Renderer& renderer = Application::getRenderer();
	renderer.updateViewport(_activeCamera, width, height);
}
