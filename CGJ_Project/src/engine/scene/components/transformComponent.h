#ifndef __engine_scene_components_transformComponent__
#define __engine_scene_components_transformComponent__


#include "engine/math/transformMatrix.h"
#include "engine/math/quaternion.h"
#include "engine/utils/coords.h"




struct TransformComponent
{
private:
	Coords3f _translation = { 0.0f, 0.0f, 0.0f };
	Quaternion _rotation = Quaternion();
	Coords3f _scale = { 1.0f, 1.0f, 1.0f };

	TransformMatrix _localTransform;
	TransformMatrix _worldTransform;
	bool _locallyUpdated = false;
	bool _globallyUpdated = false;
	bool _fixed = false;

public:
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	~TransformComponent() = default;

	inline const Coords3f& translation() const	{ return _translation; }
	inline const Quaternion& rotation() const	{ return _rotation; }
	inline const Coords3f& scale() const		{ return _scale; }

	inline const TransformMatrix& transformMatrix() const { return _worldTransform; }

	inline bool updated() const { return _locallyUpdated && _globallyUpdated; }

	inline bool fixed()	const { return _fixed; }
	inline void setFixed(bool fixed) { _fixed = fixed; }

public:
	friend class Transform;
};


#endif // !__engine_scene_components_transformComponent__