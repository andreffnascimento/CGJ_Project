#ifndef __engine_scene_components_aabbColliderComponent__
#define __engine_scene_components_aabbColliderComponent__


#include "engine/scene/components/rigidbodyComponent.h"
#include "engine/utils/coords.h"


class CollisionResolver;




struct AABBColliderComponent
{
private:
	Coords3f _initialSize = Coords3f();
	Coords3f _boundingBox = Coords3f();
	bool _fixedBoundingBox = false;
	float _restitutionCocoefficient = 1.0f;

	RigidbodyComponent* _rigidbody = nullptr;
	CollisionResolver* _collisionResolver = nullptr;

public:
	AABBColliderComponent() = delete;
	AABBColliderComponent(const AABBColliderComponent&) = default;
	AABBColliderComponent(RigidbodyComponent& rigidbody, const Coords3f& initialSize);
	~AABBColliderComponent();

	inline const Coords3f& boundingBox() const			{ return _boundingBox; }
	inline const RigidbodyComponent& rigidbody() const	{ return *_rigidbody; }
	inline CollisionResolver* collisionResolver() const { return _collisionResolver; }

	inline void setInitialSize(const Coords3f& initialSize)					{ _initialSize = initialSize / 2.0f; }
	inline void setFixedBoundingBox(bool fixedBoundingBox)					{ _fixedBoundingBox = fixedBoundingBox; }
	inline void setRestitutionCocoefficient(float restitutionCocoefficient) { _restitutionCocoefficient = restitutionCocoefficient; }

public:
	friend class PhysicsEngine;
};


#endif // !__engine_scene_components_aabbColliderComponent__