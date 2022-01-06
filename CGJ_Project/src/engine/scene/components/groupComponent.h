#ifndef __engine_scene_components_groupComponent__
#define __engine_scene_components_groupComponent__


#include <unordered_set>

#include "engine/scene/entity.h"




struct GroupComponent
{
private:
	std::unordered_set<Entity> _group = std::unordered_set<Entity>();

public:
	GroupComponent() = default;
	GroupComponent(const GroupComponent&) = default;
	~GroupComponent() = default;

	Entity addNewEntity(Scene* scene, const Entity& parentEntity);
	Entity addNewEntity(Scene* scene, const Entity& parentEntity, const std::string& tag);
	void expandGroup(std::unordered_set<Entity>& outExpandedGroup) const;

	inline const std::unordered_set<Entity>& group() const		{ return _group; }
	inline operator const std::unordered_set<Entity>& () const	{ return group(); }

	template <typename T>
	void expandGroupToComponent(std::unordered_set<T*>& outExpandedGroup) const
	{
		for (Entity entity : _group)
		{
			GroupComponent* groupComponent = entity.getComponentIfExists<GroupComponent>();
			if (groupComponent != nullptr)
				groupComponent->expandGroupToComponent(outExpandedGroup);

			if (entity.hasComponent<T>())
				outExpandedGroup.emplace(&entity.getComponent<T>());
		}
	}
};


#endif // !__engine_scene_components_groupComponent__