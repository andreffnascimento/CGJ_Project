#ifndef __ENGINE_SCENE_ECSREGISTRY_H__
#define __ENGINE_SCENE_ECSREGISTRY_H__


#include <unordered_map>




struct EntityHandle
{
public:
	static const unsigned int ENTITY_HANDLE_NULL = 0;

private:
	unsigned int _id = EntityHandle::ENTITY_HANDLE_NULL;

public:
	EntityHandle() = default;
	EntityHandle(const EntityHandle& entityHandle) = default;
	EntityHandle(unsigned int id) : _id(id) {}
	~EntityHandle() = default;

	inline bool operator==(const EntityHandle& other) const { return _id != EntityHandle::ENTITY_HANDLE_NULL && _id == other._id; }
	inline bool operator!=(const EntityHandle& other) const { return _id == EntityHandle::ENTITY_HANDLE_NULL || _id != other._id; }

	operator unsigned int() const { return _id; }
};


template <>
struct std::hash<EntityHandle>
{
	std::size_t operator()(const EntityHandle& entityHandle) const
	{
		return hash<unsigned int>()(entityHandle);
	}
};




class ECSRegistry
{

private:
	unsigned int _nextId = 1;




public:
	ECSRegistry() = default;
	ECSRegistry(const ECSRegistry& registry) = default;
	~ECSRegistry() = default;


public:
	EntityHandle createEntity();
	void destroyEntity(EntityHandle entityHandle);


public:
	inline unsigned int getNextId() const { return _nextId; }




public:
	template <typename T, typename... Args>
	T& addComponent(const EntityHandle entityHandle, Args&&... args)
	{
		auto& componentMap = getComponents<T>();
		auto iterator = componentMap.emplace(entityHandle, T(args...));
		return iterator.first->second;
	}


	template <typename T>
	T& getComponent(EntityHandle entityHandle) const
	{
		auto& componentMap = getComponents<T>();
		auto iterator = componentMap.find(entityHandle);
		return iterator->second;
	}


	template <typename T>
	bool hasComponent(EntityHandle entityHandle) const
	{
		auto& componentMap = getComponents<T>();
		auto iterator = componentMap.find(entityHandle);
		return iterator != componentMap.end();
	}



public:
	template <typename T>
	std::unordered_map<EntityHandle, T>& getComponents() const
	{
		static std::unordered_map<EntityHandle, T> componentMap = std::unordered_map<EntityHandle, T>();
		return componentMap;
	}

};


#endif // !__ENGINE_SCENE_ECSREGISTRY_H__

