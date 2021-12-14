#ifndef __ENGINE_MATH_TRANSFORM_H__
#define __ENGINE_MATH_TRANSFORM_H__


#include "engine/scene/entity.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class Transform
{

public:
	static void translate(TransformComponent& transform, const Coords3f& translation);
	static void rotate(TransformComponent& transform, const Coords3f& rotation);
	static void scale(TransformComponent& transform, const Coords3f& scale);

	static void translate(const Entity& entity, const Coords3f& translation);
	static void rotate(const Entity& entity, const Coords3f& rotation);
	static void scale(const Entity& entity, const Coords3f& scale);


public:
	static void translateTo(TransformComponent& transform, const Coords3f& newTranslation);
	static void rotateTo(TransformComponent& transform, const Coords3f& newRotation);
	static void scaleTo(TransformComponent& transform, const Coords3f& newScale);

	static void translateTo(const Entity& entity, const Coords3f& newTranslation);
	static void rotateTo(const Entity& entity, const Coords3f& newRotation);
	static void scaleTo(const Entity& entity, const Coords3f& newScale);


public:
	static void translateGroup(const Entity& entity, const Coords3f& translation);
	static void rotateGroup(const Entity& entity, const Coords3f& rotation);
	static void scaleGroup(const Entity& entity, const Coords3f& scale);

	static void translateGroupTo(const Entity& entity, const Coords3f& newTranslation);
	static void rotateGroupTo(const Entity& entity, const Coords3f& newRotation);
	static void scaleGroupTo(const Entity& entity, const Coords3f& newScale);



private:
	static void _updateGroup(const Entity& entity, const Coords3f& transformUpdate, void (*transformFunc)(TransformComponent&, const Coords3f&));
};



#endif // !__ENGINE_MATH_TRANSFORM_H__
