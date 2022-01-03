#ifndef __engine_physics_physicsEngine__
#define __engine_physics_physicsEngine__


#include "engine/physics/force.h"

#include "engine/scene/entity.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"


class Scene;




class PhysicsEngine
{

public:
	static constexpr float GRAVITY = 9.8f;
	static constexpr float AIR_DENSITY = 1.225f;
	static constexpr float DRAG_SLOW_CONSTANT = 100.0f;




private:
	unsigned int _collisionIterations = 2;




public:
	static void rotateVector(Coords3f& vector, const Coords3f& rotation);
	static void rotateVectorOnAxis(float& coord1, float& coord2, float angle);
	static void rotateBoundingBox(AABBColliderComponent& collider, const Quaternion& rotation);
	static Coords3f calculateDragForce(const Coords3f& velocity, float drag, float dragThreshold);




public:
	PhysicsEngine() = default;
	PhysicsEngine(const PhysicsEngine&) = default;
	PhysicsEngine(unsigned int collisionIterations);
	~PhysicsEngine() = default;


public:
	void initialize(const Scene& scene) const;
	void simulate(const Scene& scene, float ts) const;




private:
	void _processRigidbodyMovement(const Scene& scene, RigidbodyComponent& rigidbody, float ts) const;
	void _detectRigidbodyCollisions(const Scene& scene, EntityHandle entityId, AABBColliderComponent& collider, float ts) const;


private:
	Coords3f _calculateExpectedRotation(RigidbodyComponent& rigidbody, float ts) const;
	void _combineForces(RigidbodyComponent& rigidbody, Coords3f& linearForce, Coords3f& angularForce) const;
	void _calculateExpectedAngularVelocity(RigidbodyComponent& rigidbody, Coords3f& angularForce, float ts) const;
	void _calculateExpectedVelocity(RigidbodyComponent& rigidbody, Coords3f& linearForce, float ts, const Quaternion& rotation) const;
	void _processSleepThreshold(RigidbodyComponent& rigidbody) const;


private:
	void _resolveCollision(AABBColliderComponent& firstCollider, AABBColliderComponent& secondCollider, float ts) const;

};


#endif // !__engine_physics_physicsEngine__