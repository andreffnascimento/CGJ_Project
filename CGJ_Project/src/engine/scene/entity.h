#ifndef __ENGINE_SCENE_ENTITY_H__
#define __ENGINE_SCENE_ENTITY_H__


#include <string>
#include <unordered_set>

#include "engine/scene/ecsRegistry.h"
#include "engine/scene/scene.h"




class Entity
{

private:
	const EntityHandle _entityHandle	= EntityHandle();
	Scene* _scene						= nullptr;


	

public:
	Entity() = default;
	Entity(const Entity& entity) = default;
	Entity(const EntityHandle& entityHandle, Scene*&& scene);


public:
	inline operator EntityHandle()	const { return _entityHandle; }
	inline operator std::string()	const { return std::to_string(_entityHandle); }

	inline bool operator==(const Entity& other) const { return _entityHandle == other._entityHandle; }
	inline bool operator!=(const Entity& other) const { return _entityHandle != other._entityHandle; }




public:
	template <typename T, typename... Args>
	T& addComponent(Args&&... args) const
	{
		if (_scene == nullptr)
			throw std::string("Unable to add a component to an entity that is not part of a scene");

		T& component = _scene->_registry.addComponent<T>(_entityHandle, std::forward<Args>(args)...);
		return component;
	}


	template <typename T>
	T& getComponent() const
	{
		return _scene->_registry.getComponent<T>(_entityHandle);
	}


	template <typename T>
	bool hasComponent() const
	{
		return _scene->_registry.hasComponent<T>(_entityHandle);
	}

};




template <>
struct std::hash<Entity>
{
	std::size_t operator()(const Entity& entity) const
	{
		return hash<unsigned int>()((EntityHandle)entity);
	}
};


#endif // !__ENGINE_SCENE_ENTITY_H__