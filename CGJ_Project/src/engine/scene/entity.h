#ifndef __ENGINE_SCENE_ENTITY_H__
#define __ENGINE_SCENE_ENTITY_H__


#include <string>
#include <unordered_set>

#include "engine/scene/ecsRegistry.h"


class Scene;




class Entity
{

private:
	EntityHandle _entityHandle	= EntityHandle();
	Scene* _scene				= nullptr;


	

protected:
	Entity() = default;


public:
	Entity(const Entity&) = default;
	Entity(const EntityHandle& entityHandle, Scene*&& scene);
	virtual ~Entity() = default;


public:
	inline operator EntityHandle()	const { return _entityHandle; }
	inline operator std::string()	const { return std::to_string(_entityHandle); }

	inline bool operator==(const Entity& other) const { return _entityHandle == other._entityHandle; }
	inline bool operator!=(const Entity& other) const { return _entityHandle != other._entityHandle; }

private:
	ECSRegistry& _getSceneRegistry() const;




public:
	template <typename T, typename... Args>
	T& addComponent(Args&&... args) const
	{
		if (_scene == nullptr)
			throw std::string("Unable to add a component to an entity that is not part of a scene");

		T& component = _getSceneRegistry().addComponent<T>(_entityHandle, std::forward<Args>(args)...);
		return component;
	}


	template <typename T>
	T& getComponent() const
	{
		return _getSceneRegistry().getComponent<T>(_entityHandle);
	}


	template <typename T>
	bool hasComponent() const
	{
		return _getSceneRegistry().hasComponent<T>(_entityHandle);
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