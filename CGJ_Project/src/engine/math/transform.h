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
	typedef void (*transform_entity_coords_func_t)(TransformComponent&, RigidbodyComponent*, const Coords3f&);
	typedef void (*transform_entity_quaternion_func_t)(TransformComponent&, RigidbodyComponent*, const Quaternion&);
	typedef void (*transform_local_coords_func_t)(TransformComponent&, const Coords3f&);
	typedef void (*transform_local_quaternion_func_t)(TransformComponent&, const Quaternion&);




public:
	static inline void translateTo(const Entity& entity, const Coords3f& newTranslation)	{ _localUpdate(entity, newTranslation, Transform::_translateTo); }
	static inline void rotateTo(const Entity& entity, const Coords3f& newRotation)			{ _localUpdate(entity, newRotation, Transform::_rotateTo); }
	static inline void rotateTo(const Entity& entity, const Quaternion& newRotation)		{ _localUpdate(entity, newRotation, Transform::_rotateTo); }
	static inline void scaleTo(const Entity& entity, const Coords3f& newScale)				{ _localUpdate(entity, newScale, Transform::_scaleTo); }
		    
	static inline void translate(const Entity& entity, const Coords3f& translation)			{ _localUpdate(entity, translation, Transform::_translate); }
	static inline void rotate(const Entity& entity, const Coords3f& rotation)				{ _localUpdate(entity, rotation, Transform::_rotate); }
	static inline void rotate(const Entity& entity, const Quaternion& rotation)				{ _localUpdate(entity, rotation, Transform::_rotate); }
	static inline void scale(const Entity& entity, const Coords3f& scale)					{ _localUpdate(entity, scale, Transform::_scale); }


public:
	static inline void translateTo(TransformComponent& transform, const Coords3f& newTranslation)	{ _localUpdate(transform, newTranslation, Transform::_translateTo); }
	static inline void rotateTo(TransformComponent& transform, const Coords3f& newRotation)			{ _localUpdate(transform, newRotation, Transform::_rotateTo); }
	static inline void rotateTo(TransformComponent& transform, const Quaternion& newRotation)		{ _localUpdate(transform, newRotation, Transform::_rotateTo); }
	static inline void scaleTo(TransformComponent& transform, const Coords3f& newScale)				{ _localUpdate(transform, newScale, Transform::_scaleTo); }

	static inline void translate(TransformComponent& transform, const Coords3f& translation)		{ _localUpdate(transform, translation, Transform::_translate); }
	static inline void rotate(TransformComponent& transform, const Coords3f& rotation)				{ _localUpdate(transform, rotation, Transform::_rotate); }
	static inline void rotate(TransformComponent& transform, const Quaternion& rotation)			{ _localUpdate(transform, rotation, Transform::_rotate); }
	static inline void scale(TransformComponent& transform, const Coords3f& scale)					{ _localUpdate(transform, scale, Transform::_scale); }


public:
	static const TransformMatrix& calculateTransformMatrix(const Entity& entity);
	static const TransformMatrix& calculateTransformMatrix(TransformComponent& transform);	// only considers local transformations
	static void decomposeTransformMatrix(const Entity& entity, Coords3f& outTranslation, Quaternion& outRotation, Coords3f& outScale);
	static void decomposeTransformMatrix(const TransformComponent& transform, Coords3f& outTranslation, Quaternion& outRotation, Coords3f& outScale);


private:
	static void _localUpdate(const Entity& entity, const Coords3f& transform, transform_entity_coords_func_t transformFunc);
	static void _localUpdate(const Entity& entity, const Quaternion& transform, transform_entity_quaternion_func_t transformFunc);
	static void _localUpdate(TransformComponent& transformComponent, const Coords3f& transform, transform_local_coords_func_t transformFunc);
	static void _localUpdate(TransformComponent& transformComponent, const Quaternion& transform, transform_local_quaternion_func_t transformFunc);
	static void _groupUpdate(const Entity& entity);


private:
	static void _translateTo(TransformComponent& transform, RigidbodyComponent* rigidbody, const Coords3f& newTranslation);
	static void _rotateTo(TransformComponent& transform, RigidbodyComponent* rigidbody, const Coords3f& newRotation);
	static void _rotateTo(TransformComponent& transform, RigidbodyComponent* rigidbody, const Quaternion& newRotation);
	static void _scaleTo(TransformComponent& transform, RigidbodyComponent* rigidbody, const Coords3f& newScale);

	static void _translate(TransformComponent& transform, RigidbodyComponent* rigidbody, const Coords3f& translation);
	static void _rotate(TransformComponent& transform, RigidbodyComponent* rigidbody, const Coords3f& rotation);
	static void _rotate(TransformComponent& transform, RigidbodyComponent* rigidbody, const Quaternion& rotation);
	static void _scale(TransformComponent& transform, RigidbodyComponent* rigidbody, const Coords3f& scale);


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