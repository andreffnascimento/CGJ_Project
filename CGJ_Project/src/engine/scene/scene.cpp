#include "scene.h"

#include <unordered_map>

#include "engine/scene/components.h"

#include "engine/app/application.h"
#include "engine/math/transform.h"
#include "engine/physics/physicsEngine.h"
#include "engine/renderer/renderer.h"
#include "engine/utils/coords.h"




void Scene::onCreate()
{
	// initialize all the script components
	for (auto& script : _registry.getComponents<ScriptComponent>())
		script.second.onCreate();

	// initialize the transform matrices
	for (auto& transformIterator : _registry.getComponents<TransformComponent>())
		Transform::calculateTransformMatrix(Entity(transformIterator.first, this));

	PhysicsEngine& physicsEngine = Application::getPhysicsEngine();
	physicsEngine.initialize(*this);
}


void Scene::onFixedUpdate(float ts)		// prevents the physics engine to break when the ts is too large
{
	// update entity scripts
	for (auto& scriptIterator : _registry.getComponents<ScriptComponent>())
		scriptIterator.second.onUpdate(ts);

	// simulate physics
	PhysicsEngine& physicsEngine = Application::getPhysicsEngine();
	physicsEngine.simulate(*this, ts);
}


void Scene::onUpdate(float ts)
{
	_timeAccumulator += ts;
	while (_timeAccumulator >= PhysicsEngine::PHYSICS_SIMULATION_MIN_INTERVAL)
	{
		float fixedTs = std::min(ts, PhysicsEngine::PHYSICS_SIMULATION_MAX_INTERVAL);
		_timeAccumulator -= fixedTs;
		onFixedUpdate(fixedTs);
	}

	PhysicsEngine& physicsEngine = Application::getPhysicsEngine();
	physicsEngine.updateTransforms(*this);

	// update entity transform matrixes
	for (auto& transformIterator : _registry.getComponents<TransformComponent>())
		Transform::calculateTransformMatrix(Entity(transformIterator.first, this));

	// render to the screen
	Renderer& renderer = Application::getRenderer();
	renderer.renderScene(*this);
}


void Scene::onViewportResize(int width, int height)
{
	Renderer& renderer = Application::getRenderer();
	renderer.updateViewport(_activeCamera, width, height);
}




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