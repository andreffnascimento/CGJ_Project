#include "entity.h"

#include "engine/scene/components.h"
#include "engine/scene/scene.h"




Entity::Entity(const EntityHandle& entityHandle, const Scene*&& scene)
	: _entityHandle(EntityHandle(entityHandle)), _scene(scene)
{
	// empty
}




TagComponent& Entity::tag() const
{
	return _getSceneRegistry().getComponent<TagComponent>(_entityHandle);
}

TransformComponent& Entity::transform() const
{
	return _getSceneRegistry().getComponent<TransformComponent>(_entityHandle);
}




const Scene& Entity::_getScene() const
{
	return *_scene;
}

const ECSRegistry& Entity::_getSceneRegistry() const
{
	return _scene->_registry;
}
