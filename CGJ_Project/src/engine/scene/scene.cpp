#include "scene.h"

#include <unordered_map>

#include "engine/scene/components.h"

#include "engine/app/application.h"
#include "engine/math/transform.h"
#include "engine/physics/physicsEngine.h"
#include "engine/renderer/renderer.h"




Entity Scene::createEntity()
{
	std::string tag = "Entity$<" + std::to_string(_registry.getNextId()) + ">";
	if (hasEntityWithTag(tag))
		throw std::string("There is already an entity with the specified tag: " + tag);

	Entity entity = Entity(_registry.createEntity(), this);
	entity.addComponent<TagComponent>(tag);
	entity.addComponent<TransformComponent>();
	return entity;
}

Entity Scene::createEntity(const std::string& tag, bool joinId)
{
	std::string fixedTag = tag + (joinId ? "$<" + std::to_string(_registry.getNextId()) + ">" : "");
	if (hasEntityWithTag(fixedTag))
		throw std::string("There is already an entity with the specified tag: " + fixedTag);

	Entity entity = Entity(_registry.createEntity(), this);
	entity.addComponent<TagComponent>(fixedTag);
	entity.addComponent<TransformComponent>();
	return entity;
}



bool Scene::hasEntityWithTag(const std::string& tag) const
{
	auto& taggedEntities = _registry.getComponents<TagComponent>();
	for (auto& iterator : taggedEntities)
		if (iterator.second == tag)
			return true;
	return false;
}


Entity Scene::getEntityById(const EntityHandle& entityHandle) const
{
	if (_registry.getNextId() < entityHandle)
		throw std::string("There is no entity with the provided id: " + (unsigned int)entityHandle);

	return Entity(entityHandle, this);
}


Entity Scene::getEntityByTag(const std::string& tag) const
{
	auto& taggedEntities = _registry.getComponents<TagComponent>();
	for (auto& iterator : taggedEntities)
		if (iterator.second == tag)
			return Entity(iterator.first, this);

	throw std::string("There is no entity with the provided tag: " + tag);
}


std::unordered_set<Entity> Scene::getEntitiesByTag(const std::regex& regex) const
{
	auto& taggedEntities = _registry.getComponents<TagComponent>();
	auto entities = std::unordered_set<Entity>();
	for (auto& iterator : taggedEntities)
		if (iterator.second == regex)
			entities.emplace(Entity(iterator.first, this));
	return entities;
}




void Scene::setActiveCamera(const CameraEntity& camera)
{
	_activeCamera = camera;

	Application& application = Application::getInstance();
	WindowCoords windowCoords = application.getWindowSize();
	onViewportResize(windowCoords.x, windowCoords.y);
}


void Scene::setReflectionCoefficients(float ambient, float diffuse, float specular)
{
	if (ambient < 0.0f || ambient > 1.0f)
		throw std::string("Ambient coefficient must be a float value between 0.0f and 1.0f");

	if (diffuse < 0.0f || diffuse > 1.0f)
		throw std::string("Diffuse coefficient must be a float value between 0.0f and 1.0f");

	if (specular < 0.0f || specular > 1.0f)
		throw std::string("Specular coefficient must be a float value between 0.0f and 1.0f");

	_reflectionCoefficients = { ambient, diffuse, specular };
}




void Scene::onCreate()
{
	// initialize all the script components
	for (auto& script : _registry.getComponents<ScriptComponent>())
		script.second.onCreate();

	PhysicsEngine& physicsEngine = Application::getPhysicsEngine();
	physicsEngine.initialize(*this);
}


void Scene::onUpdate(float ts)
{
	// update entity scripts
	for (auto& scriptIterator : _registry.getComponents<ScriptComponent>())
		scriptIterator.second.onUpdate(ts);

	// simulate physics
	PhysicsEngine& physicsEngine = Application::getPhysicsEngine();
	physicsEngine.simulate(*this, ts);
	
	// update entity transform matrixes
	for (auto& transformIterator : _registry.getComponents<TransformComponent>())
		Transform::calculateTransformMatrix(Entity(transformIterator.first, this));

	// render to the screen
	Renderer& renderer = Application::getRenderer();
	renderer.initSceneRendering();
	renderer.renderCamera(*this);
	renderer.renderLights(*this);
	renderer.renderMeshes(*this);
	renderer.renderColliders(*this);
}


void Scene::onViewportResize(int width, int height)
{
	Renderer& renderer = Application::getRenderer();
	renderer.updateViewport(_activeCamera, width, height);
}
