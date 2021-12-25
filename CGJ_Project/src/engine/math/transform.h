#ifndef __engine_math_transform__
#define __engine_math_transform__


#include <unordered_set>

#include "engine/scene/entity.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class Transform
{

public:
	typedef void (*transform_func_t)(TransformComponent&, const Coords3f&);




public:
	static void translateTo(const Entity& entity, const Coords3f& newTranslation);
	static void rotateTo(const Entity& entity, const Coords3f& newRotation);
	static void scaleTo(const Entity& entity, const Coords3f& newScale);

	static void translate(const Entity& entity, const Coords3f& translation);
	static void rotate(const Entity& entity, const Coords3f& rotation);
	static void scale(const Entity& entity, const Coords3f& scale);


public:
	static const TransformMatrix& calculateTransformMatrix(const Entity& entity);


private:
	static void _localUpdate(const Entity& entity, const Coords3f& transform, transform_func_t transformFunc);
	static void _groupUpdate(const Entity& entity);


private:
	static void _translateTo(TransformComponent& transform, const Coords3f& newTranslation);
	static void _rotateTo(TransformComponent& transform, const Coords3f& newRotation);
	static void _scaleTo(TransformComponent& transform, const Coords3f& newScale);

	static void _translate(TransformComponent& transform, const Coords3f& translation);
	static void _rotate(TransformComponent& transform, const Coords3f& rotation);
	static void _scale(TransformComponent& transform, const Coords3f& scale);

};


#endif // !__engine_math_transform__