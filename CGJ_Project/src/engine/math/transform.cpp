#include "transform.h"


typedef Transform::transform_func_coords_t transform_func_coords_t;
typedef Transform::transform_func_quaternion_t transform_func_quaternion_t;




void Transform::translateTo(const Entity& entity, const Coords3f& newTranslation)
{
	_localUpdate(entity, newTranslation, Transform::_translateTo);
}

void Transform::rotateTo(const Entity& entity, const Coords3f& newRotation)
{
	_localUpdate(entity, newRotation, Transform::_rotateTo);
}

void Transform::rotateTo(const Entity& entity, const Quaternion& newRotation)
{
	_localUpdate(entity, newRotation, Transform::_rotateTo);
}

void Transform::scaleTo(const Entity& entity, const Coords3f& newScale)
{
	_localUpdate(entity, newScale, Transform::_scaleTo);
}


void Transform::translate(const Entity& entity, const Coords3f& translation)
{
	_localUpdate(entity, translation, Transform::_translate);
}

void Transform::rotate(const Entity& entity, const Coords3f& rotation)
{
	_localUpdate(entity, rotation, Transform::_rotate);
}

void Transform::rotate(const Entity& entity, const Quaternion& rotation)
{
	_localUpdate(entity, rotation, Transform::_rotate);
}

void Transform::scale(const Entity& entity, const Coords3f& scale)
{
	_localUpdate(entity, scale, Transform::_scale);
}




const TransformMatrix& Transform::calculateTransformMatrix(const Entity& entity)
{
	TransformComponent& transform = entity.transform();
	if (transform._locallyUpdated && transform._globallyUpdated)
		return transform._worldTransform;

	if (!transform._locallyUpdated)
		transform._localTransform.calculateTransformMatrix(transform.translation(), transform.rotation(), transform.scale());

	ParentComponent* parent = entity.getComponentIfExists<ParentComponent>();
	if (parent != nullptr)
	{
		const TransformMatrix& parentTransform = Transform::calculateTransformMatrix(parent->parent());
		transform._worldTransform = parentTransform * transform._localTransform;
	}
	else
	{
		transform._worldTransform = transform._localTransform;
	}

	return transform._worldTransform;
}




void Transform::_localUpdate(const Entity& entity, const Coords3f& transform, transform_func_coords_t transformFunc)
{
	TransformComponent& transformComponent = entity.transform();
	transformComponent._locallyUpdated = false;
	transformFunc(transformComponent, transform);
	_groupUpdate(entity);
}

void Transform::_localUpdate(const Entity& entity, const Quaternion& transform, transform_func_quaternion_t transformFunc)
{
	TransformComponent& transformComponent = entity.transform();
	transformComponent._locallyUpdated = false;
	transformFunc(transformComponent, transform);
	_groupUpdate(entity);
}

void Transform::_groupUpdate(const Entity& entity)
{
	const GroupComponent* group = entity.getComponentIfExists<GroupComponent>();
	if (group == nullptr)
		return;

	std::unordered_set<TransformComponent*> groupTransforms = std::unordered_set<TransformComponent*>();
	group->expandGroupToComponent<TransformComponent>(groupTransforms);
	for (auto& transform : groupTransforms)
		transform->_globallyUpdated = false;
}




void Transform::_translateTo(TransformComponent& transform, const Coords3f& newTranslation)
{
	transform._translation = newTranslation;
}

void Transform::_rotateTo(TransformComponent& transform, const Coords3f& newRotation)
{
	transform._rotation = Quaternion(newRotation);
}

void Transform::_rotateTo(TransformComponent& transform, const Quaternion& newRotation)
{
	transform._rotation = newRotation;
}

void Transform::_scaleTo(TransformComponent& transform, const Coords3f& newScale)
{
	transform._scale = newScale;
}


void Transform::_translate(TransformComponent& transform, const Coords3f& translation)
{
	transform._translation += translation;
}

void Transform::_rotate(TransformComponent& transform, const Coords3f& rotation)
{
	transform._rotation.rotate(rotation);
}

void Transform::_rotate(TransformComponent& transform, const Quaternion& rotation)
{
	transform._rotation += rotation;
}

void Transform::_scale(TransformComponent& transform, const Coords3f& scale)
{
	transform._scale *= scale;
}