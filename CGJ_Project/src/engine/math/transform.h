#ifndef __engine_math_transform__
#define __engine_math_transform__


#include <unordered_set>

#include "engine/scene/entity.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class Transform
{

public:
	static constexpr double PI = 3.14159265358979323846;

	typedef void (*transform_func_t)(const Entity&, const Coords3f&);




public:
	static void translateTo(const Entity& entity, const Coords3f& newTranslation, bool applyToGroup = true);
	static void rotateTo(const Entity& entity, const Coords3f& newRotation, bool applyToGroup = true);
	static void scaleTo(const Entity& entity, const Coords3f& newScale, bool applyToGroup = true);

	static void translate(const Entity& entity, const Coords3f& translation, bool applyToGroup = true);
	static void rotate(const Entity& entity, const Coords3f& rotation, bool applyToGroup = true);
	static void scale(const Entity& entity, const Coords3f& scale, bool applyToGroup = true);




private:
	static void _singleTranslateTo(const Entity& entity, const Coords3f& newTranslation);
	static void _singleRotateTo(const Entity& entity, const Coords3f& newRotation);
	static void _singleScaleTo(const Entity& entity, const Coords3f& newScale);

	static void _singleTranslate(const Entity& entity, const Coords3f& translation);
	static void _singleRotate(const Entity& entity, const Coords3f& rotation);
	static void _singleScale(const Entity& entity, const Coords3f& scale);


private:
	static void _groupTranslateTo(const Entity& entity, const Coords3f& newTranslation);
	static void _groupRotateTo(const Entity& entity, const Coords3f& newRotation);
	static void _groupScaleTo(const Entity& entity, const Coords3f& newScale);

	static void _groupTranslate(const Entity& entity, const Coords3f& translation);
	static void _groupRotate(const Entity& entity, const Coords3f& rotation);
	static void _groupScale(const Entity& entity, const Coords3f& scale);


private:
	static void _translate(TransformComponent& transform, const Coords3f& translation);
	static void _rotate(TransformComponent& transform, const Coords3f& rotation);
	static void _scale(TransformComponent& transform, const Coords3f& scale);


private:
	static transform_func_t _applyToGroupRedirect(const Entity& entity, bool applyToGroup, transform_func_t groupTransformFunc, transform_func_t singleTransformFunc);
	static void _updateGroup(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& transformUpdate, void (*transformFunc)(TransformComponent&, const Coords3f&));


private:
	static void _scaleGroup(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, const Coords3f& scale);


private:
	static void _rotateGroup(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, const Coords3f& rotation);
	static void _rotateGroupXAxis(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, double radians);
	static void _rotateGroupYAxis(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, double radians);
	static void _rotateGroupZAxis(std::unordered_set<TransformComponent*>& expandedGroup, const Coords3f& originalPosition, double radians);
	static void _rotateInAxis(float& coord1, float& coord2, double angleCos, double angleSin);

};



#endif // !__engine_math_transform__