#ifndef __engine_scene_components_parentComponent__
#define __engine_scene_components_parentComponent__


#include "engine/scene/entity.h"




struct ParentComponent
{
private:
	Entity _parent = Entity();

public:
	ParentComponent() = delete;
	ParentComponent(const ParentComponent&) = default;
	ParentComponent(const Entity& parent) : _parent(parent) {}
	~ParentComponent() = default;

	inline void setParent(const Entity& parent) { _parent = parent; }

	inline const Entity& parent() const		{ return _parent; }
	inline operator const Entity& () const	{ return parent(); }
};


#endif // !__engine_scene_components_parentComponent__