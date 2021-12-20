#include "transform.h"

#include "engine/math/AVTmathLib.h"



typedef Transform::transform_func_t transform_func_t;




void Transform::translateTo(const Entity& entity, const Coords3f& newTranslation, bool applyToGroup)
{
	auto transformFunc = _applyToGroupRedirect(entity, applyToGroup, &Transform::_groupTranslateTo, &Transform::_singleTranslateTo);
	transformFunc(entity, newTranslation);
}

void Transform::rotateTo(const Entity& entity, const Coords3f& newRotation, bool applyToGroup)
{
	auto transformFunc = _applyToGroupRedirect(entity, applyToGroup, &Transform::_groupRotateTo, &Transform::_singleRotateTo);
	transformFunc(entity, newRotation);
}

void Transform::scaleTo(const Entity& entity, const Coords3f& newScale, bool applyToGroup)
{
	auto transformFunc = _applyToGroupRedirect(entity, applyToGroup, &Transform::_groupScaleTo, &Transform::_singleScaleTo);
	transformFunc(entity, newScale);
}


void Transform::translate(const Entity& entity, const Coords3f& translation, bool applyToGroup)
{
	auto transformFunc = _applyToGroupRedirect(entity, applyToGroup, &Transform::_groupTranslate, &Transform::_singleTranslate);
	transformFunc(entity, translation);
}

void Transform::rotate(const Entity& entity, const Coords3f& rotation, bool applyToGroup)
{
	auto transformFunc = _applyToGroupRedirect(entity, applyToGroup, &Transform::_groupRotate, &Transform::_singleRotate);
	transformFunc(entity, rotation);
}

void Transform::scale(const Entity& entity, const Coords3f& scale, bool applyToGroup)
{
	auto transformFunc = _applyToGroupRedirect(entity, applyToGroup, &Transform::_groupScale, &Transform::_singleScale);
	transformFunc(entity, scale);
}




void Transform::_singleTranslateTo(const Entity& entity, const Coords3f& newTranslation)
{
	entity.transform()._translation = newTranslation;
}

void Transform::_singleRotateTo(const Entity& entity, const Coords3f& newRotation)
{
	entity.transform()._rotation = newRotation;
}

void Transform::_singleScaleTo(const Entity& entity, const Coords3f& newScale)
{
	entity.transform()._scale = newScale;
}


void Transform::_singleTranslate(const Entity& entity, const Coords3f& translation)
{
	entity.transform()._translation += translation;
}

void Transform::_singleRotate(const Entity& entity, const Coords3f& rotation)
{
	entity.transform()._rotation += rotation;
}

void Transform::_singleScale(const Entity& entity, const Coords3f& scale)
{
	entity.transform()._scale *= scale;
}




void Transform::_groupTranslateTo(const Entity& entity, const Coords3f& newTranslation)
{
	Coords3f translation = newTranslation - entity.transform()._translation;
	Transform::_groupTranslate(entity, translation);
}

void Transform::_groupRotateTo(const Entity& entity, const Coords3f& newRotation)
{
	Coords3f rotation = newRotation - entity.transform()._rotation;
	Transform::_groupTranslate(entity, rotation);
}

void Transform::_groupScaleTo(const Entity& entity, const Coords3f& newScale)
{
	Coords3f scale = newScale / entity.transform()._scale;
	Transform::_groupTranslate(entity, scale);
}


void Transform::_groupTranslate(const Entity& entity, const Coords3f& translation)
{
	TransformComponent& mainEntityTransform = entity.transform();
	GroupComponent& mainEntityGroup = entity.getComponent<GroupComponent>();
	Transform::_translate(mainEntityTransform, translation);

	std::unordered_set<TransformComponent*> expandedGroup = std::unordered_set<TransformComponent*>();
	mainEntityGroup.expandGroupToComponent<TransformComponent>(expandedGroup);
	Transform::_updateGroup(expandedGroup, translation, Transform::_translate);

}

void Transform::_groupRotate(const Entity& entity, const Coords3f& rotation)
{
	TransformComponent& mainEntityTransform = entity.transform();
	GroupComponent& mainEntityGroup = entity.getComponent<GroupComponent>();
	Transform::_rotate(mainEntityTransform, rotation);

	std::unordered_set<TransformComponent*> expandedGroup = std::unordered_set<TransformComponent*>();
	mainEntityGroup.expandGroupToComponent<TransformComponent>(expandedGroup);
	Transform::_updateGroup(expandedGroup, rotation, Transform::_rotate);
	Transform::_rotateGroup(expandedGroup, mainEntityTransform._translation, rotation);
}

void Transform::_groupScale(const Entity& entity, const Coords3f& scale)
{
	TransformComponent& mainEntityTransform = entity.transform();
	GroupComponent& mainEntityGroup = entity.getComponent<GroupComponent>();
	Transform::_scale(mainEntityTransform, scale);

	std::unordered_set<TransformComponent*> expandedGroup = std::unordered_set<TransformComponent*>();
	mainEntityGroup.expandGroupToComponent<TransformComponent>(expandedGroup);
	Transform::_updateGroup(expandedGroup, scale, Transform::_scale);
	Transform::_scaleGroup(expandedGroup, mainEntityTransform._translation, scale);
}




void Transform::_translate(TransformComponent& transform, const Coords3f& translation)
{
	transform._translation += translation;
}

void Transform::_rotate(TransformComponent& transform, const Coords3f& rotation)
{
	transform._rotation += rotation;
}

void Transform::_scale(TransformComponent& transform, const Coords3f& scale)
{
	transform._scale *= scale;
}




transform_func_t Transform::_applyToGroupRedirect(const Entity& entity, bool applyToGroup, transform_func_t groupTransformFunc, transform_func_t singleTransformFunc)
{
	return (applyToGroup && entity.hasComponent<GroupComponent>() ? groupTransformFunc : singleTransformFunc);
}

void Transform::_updateGroup(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& transformUpdate, void(*transformFunc)(TransformComponent&, const Coords3f&))
{
	for (TransformComponent *transform : expandedGroup)
		transformFunc(*transform, transformUpdate);
}




void Transform::_scaleGroup(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, const Coords3f& scale)
{
	for (TransformComponent* transform : expandedGroup)
	{
		transform->_translation.x += (transform->_translation.x - originalPosition.x) * (scale.x - 1.0f);
		transform->_translation.y += (transform->_translation.y - originalPosition.y) * (scale.y - 1.0f);
		transform->_translation.z += (transform->_translation.z - originalPosition.z) * (scale.z - 1.0f);
	}
}




void Transform::_rotateGroup(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, const Coords3f& rotation)
{
	for (TransformComponent* transform : expandedGroup)
		transform->_translation -= originalPosition;

	if (rotation.x != 0.0f)
		_rotateGroupXAxis(expandedGroup, originalPosition, rotation.x * (Transform::PI / 180.0));

	if (rotation.y != 0.0f)
		_rotateGroupYAxis(expandedGroup, originalPosition, rotation.y * (Transform::PI / 180.0));

	if (rotation.z != 0.0f)
		_rotateGroupZAxis(expandedGroup, originalPosition, rotation.z * (Transform::PI / 180.0));

	for (TransformComponent* transform : expandedGroup)
		transform->_translation += originalPosition;
}

void Transform::_rotateGroupXAxis(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, double radians)
{
	double angleCos = std::cos(radians);
	double angleSin = std::sin(radians);

	for (TransformComponent* transform : expandedGroup)
		_rotateInAxis(transform->_translation.y, transform->_translation.z, angleCos, angleSin);
}

void Transform::_rotateGroupYAxis(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, double radians)
{
	double angleCos = std::cos(radians);
	double angleSin = std::sin(radians);

	for (TransformComponent* transform : expandedGroup)
		_rotateInAxis(transform->_translation.z, transform->_translation.x, angleCos, angleSin);
}

void Transform::_rotateGroupZAxis(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, double radians)
{
	double angleCos = std::cos(radians);
	double angleSin = std::sin(radians);
	
	for (TransformComponent* transform : expandedGroup)
		_rotateInAxis(transform->_translation.x, transform->_translation.y, angleCos, angleSin);
}

void Transform::_rotateInAxis(float& coord1, float& coord2, double angleCos, double angleSin)
{
	float originalCoord1 = coord1;
	float originalCoord2 = coord2;

	coord1 = (float)(originalCoord1 * angleCos - originalCoord2 * angleSin);
	coord2 = (float)(originalCoord1 * angleSin + originalCoord2 * angleCos);
}