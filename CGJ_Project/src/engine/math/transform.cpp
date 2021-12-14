#include "transform.h"




void Transform::translate(TransformComponent& transform, const Coords3f& translation)
{
	transform._translation += translation;
}

void Transform::rotate(TransformComponent& transform, const Coords3f& rotation)
{
	transform._rotation += rotation;
}

void Transform::scale(TransformComponent& transform, const Coords3f& scale)
{
	transform._scale += scale;
}


void Transform::translate(const Entity& entity, const Coords3f& translation)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Transform::translate(transform, translation);
}

void Transform::rotate(const Entity& entity, const Coords3f& rotation)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Transform::rotate(transform, rotation);
}

void Transform::scale(const Entity& entity, const Coords3f& scale)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Transform::scale(transform, scale);
}




void Transform::translateTo(TransformComponent& transform, const Coords3f& newTranslation)
{
	transform._translation = newTranslation;
}

void Transform::rotateTo(TransformComponent& transform, const Coords3f& newRotation)
{
	transform._rotation = newRotation;
}

void Transform::scaleTo(TransformComponent& transform, const Coords3f& newScale)
{
	transform._scale = newScale;
}


void Transform::translateTo(const Entity& entity, const Coords3f& newTranslation)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Transform::translateTo(transform, newTranslation);
}

void Transform::rotateTo(const Entity& entity, const Coords3f& newRotation)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Transform::rotateTo(transform, newRotation);
}

void Transform::scaleTo(const Entity& entity, const Coords3f& newScale)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Transform::scaleTo(transform, newScale);
}




void Transform::translateGroup(const Entity& entity, const Coords3f& translation)
{
	_updateGroup(entity, translation, &Transform::translate);
}

void Transform::rotateGroup(const Entity& entity, const Coords3f& rotation)
{
	_updateGroup(entity, rotation, &Transform::rotate);
}

void Transform::scaleGroup(const Entity& entity, const Coords3f& scale)
{
	_updateGroup(entity, scale, &Transform::scale);
}


void Transform::translateGroupTo(const Entity& entity, const Coords3f& newTranslation)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Coords3f translation = newTranslation - transform._translation;
	_updateGroup(entity, translation, &Transform::translate);
}

void Transform::rotateGroupTo(const Entity& entity, const Coords3f& newRotation)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Coords3f rotation = newRotation - transform._rotation;
	_updateGroup(entity, rotation, &Transform::rotate);
}

void Transform::scaleGroupTo(const Entity& entity, const Coords3f& newScale)
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	Coords3f scale = newScale - transform._scale;
	_updateGroup(entity, scale, &Transform::scale);
}




void Transform::_updateGroup(const Entity& entity, const Coords3f& transformUpdate, void (*transformFunc)(TransformComponent&, const Coords3f&))
{
	TransformComponent& transform = entity.getComponent<TransformComponent>();
	transformFunc(transform, transformUpdate);

	if (entity.hasComponent<GroupComponent>())
	{
		GroupComponent& group = entity.getComponent<GroupComponent>();
		for (auto& entity : group.getGroup())
			_updateGroup(entity, transformUpdate, transformFunc);
	}
}
