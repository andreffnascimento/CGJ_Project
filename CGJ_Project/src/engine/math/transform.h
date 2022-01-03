#ifndef __engine_math_transform__
#define __engine_math_transform__


#include "engine/math/quaternion.h"
#include "engine/math/transformMatrix.h"

#include "engine/scene/entity.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class Transform
{

public:
	typedef void (*transform_func_coords_t)(TransformComponent&, const Coords3f&);
	typedef void (*transform_func_quaternion_t)(TransformComponent&, const Quaternion&);




public:
	static void translateTo(const Entity& entity, const Coords3f& newTranslation);
	static void rotateTo(const Entity& entity, const Coords3f& newRotation);
	static void rotateTo(const Entity& entity, const Quaternion& newRotation);
	static void scaleTo(const Entity& entity, const Coords3f& newScale);

	static void translate(const Entity& entity, const Coords3f& translation);
	static void rotate(const Entity& entity, const Coords3f& rotation);
	static void rotate(const Entity& entity, const Quaternion& rotation);
	static void scale(const Entity& entity, const Coords3f& scale);


public:
	static const TransformMatrix& calculateTransformMatrix(const Entity& entity);
	static void decomposeTransformMatrix(const Entity& entity, Coords3f& outTranslation, Quaternion& outRotation, Coords3f& outScale);


private:
	static void _localUpdate(const Entity& entity, const Coords3f& transform, transform_func_coords_t transformFunc);
	static void _localUpdate(const Entity& entity, const Quaternion& transform, transform_func_quaternion_t transformFunc);
	static void _groupUpdate(const Entity& entity);


private:
	static void _translateTo(TransformComponent& transform, const Coords3f& newTranslation);
	static void _rotateTo(TransformComponent& transform, const Coords3f& newRotation);
	static void _rotateTo(TransformComponent& transform, const Quaternion& newRotation);
	static void _scaleTo(TransformComponent& transform, const Coords3f& newScale);

	static void _translate(TransformComponent& transform, const Coords3f& translation);
	static void _rotate(TransformComponent& transform, const Coords3f& rotation);
	static void _rotate(TransformComponent& transform, const Quaternion& rotation);
	static void _scale(TransformComponent& transform, const Coords3f& scale);

};


#endif // !__engine_math_transform__