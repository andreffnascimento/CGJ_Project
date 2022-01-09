#include "transform.h"

typedef Transform::transform_coords_func_t		transform_coords_func_t;
typedef Transform::transform_quaternion_func_t	transform_quaternion_func_t;




bool Transform::updated(const Entity& entity)
{
	return updated(entity.transform());
}

bool Transform::updated(const TransformComponent& transform)
{
	return transform._locallyUpdated && transform._globallyUpdated;
}


const TransformMatrix& Transform::calculateTransformMatrix(const Entity& entity)
{
	TransformComponent& transform = entity.transform();
	if (transform._locallyUpdated && transform._globallyUpdated)
		return transform._worldTransform;

	if (!transform._locallyUpdated)
	{
		transform._localTransform.calculateTransformMatrix(transform.translation(), transform.rotation(), transform.scale());
		transform._locallyUpdated = true;
	}

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

	transform._globallyUpdated = true;
	return transform._worldTransform;
}

const TransformMatrix& Transform::calculateTransformMatrix(TransformComponent& transform)
{
	if (transform._locallyUpdated)
		return transform._localTransform;

	transform._localTransform.calculateTransformMatrix(transform.translation(), transform.rotation(), transform.scale());
	transform._locallyUpdated = true;
	return transform._localTransform;
}


void Transform::decomposeTransformMatrix(const Entity& entity, Coords3f& outTranslation, Quaternion& outRotation, Coords3f& outScale)
{
	return decomposeTransformMatrix(entity.transform(), outTranslation, outRotation, outScale);
}

void Transform::decomposeTransformMatrix(const TransformComponent& transform, Coords3f& outTranslation, Quaternion& outRotation, Coords3f& outScale)
{
	TransformMatrix transformMatrix = transform.transformMatrix();
	transformMatrix.transpose();

	// extract translation vector
	outTranslation = { transformMatrix[0][3], transformMatrix[1][3], transformMatrix[2][3] };

	// extract scale vector
	transformMatrix[0][3] = transformMatrix[1][3] = transformMatrix[2][3] = 0.0f;
	outScale.x = Coords3f({ transformMatrix[0][0], transformMatrix[1][0], transformMatrix[2][0] }).length();
	outScale.y = Coords3f({ transformMatrix[0][1], transformMatrix[1][1], transformMatrix[2][1] }).length();
	outScale.z = Coords3f({ transformMatrix[0][2], transformMatrix[1][2], transformMatrix[2][2] }).length();

	// extract the rotation vector
	transformMatrix[0][0] /= outScale.x;	transformMatrix[0][1] /= outScale.y;	transformMatrix[0][2] /= outScale.z;
	transformMatrix[1][0] /= outScale.x;	transformMatrix[1][1] /= outScale.y;	transformMatrix[1][2] /= outScale.z;
	transformMatrix[2][0] /= outScale.x;	transformMatrix[2][1] /= outScale.y;	transformMatrix[2][2] /= outScale.z;
	outRotation = Quaternion(transformMatrix);
}




void Transform::_localUpdate(const Entity& entity, const Coords3f& transform, transform_coords_func_t transformFunc)
{
	_localUpdate(entity.transform(), transform, transformFunc);
	_groupUpdate(entity);
}

void Transform::_localUpdate(const Entity& entity, const Quaternion& transform, transform_quaternion_func_t transformFunc)
{
	_localUpdate(entity.transform(), transform, transformFunc);
	_groupUpdate(entity);
}

void Transform::_localUpdate(TransformComponent& transformComponent, const Coords3f& transform, transform_coords_func_t transformFunc)
{
	transformFunc(transformComponent, transform);
	transformComponent._locallyUpdated = false;
}

void Transform::_localUpdate(TransformComponent& transformComponent, const Quaternion& transform, transform_quaternion_func_t transformFunc)
{
	transformFunc(transformComponent, transform);
	transformComponent._locallyUpdated = false;
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