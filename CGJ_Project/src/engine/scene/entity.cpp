#include "entity.h"

#include "engine/scene/scene.h"




Entity::Entity(const EntityHandle& entityHandle, Scene*&& scene)
	: _entityHandle(EntityHandle(entityHandle)), _scene(scene)
{
	// empty
}




ECSRegistry& Entity::_getSceneRegistry() const
{
	return _scene->_registry;
}
