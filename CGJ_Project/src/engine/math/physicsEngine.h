#ifndef __engine_math_physicsEngine__
#define __engine_math_physicsEngine__


#include "engine/math/force.h"
#include "engine/scene/entity.h"
#include "engine/scene/components.h"


class Scene;




class PhysicsEngine
{

public:
	static constexpr float GRAVITY = 9.8f;
	static constexpr float AIR_DENSITY = 1.225f;
	static constexpr float DRAG_SLOW_CONSTANT = 100.0f;




public:
	static void rotateVector(Coords3f& vector, const Coords3f& rotation);
	static void rotateVectorOnAxis(float& coord1, float& coord2, float angle);
	static Coords3f calculateDragForce(const Coords3f& velocity, float drag, float dragThreshold);




public:
	PhysicsEngine() = default;
	PhysicsEngine(const PhysicsEngine&) = default;
	~PhysicsEngine() = default;


public:
	void run(const Scene& scene, float ts) const;




private:
	void _processRigidbody(const Scene& scene, EntityHandle entityId, RigidbodyComponent& rigidbody, float ts) const;


private:
	Coords3f _calculateExpectedRotation(RigidbodyComponent& rigidbody, float ts) const;
	void _combineForces(RigidbodyComponent& rigidbody, Coords3f& linearForce, Coords3f& angularForce) const;
	void _calculateExpectedAngularVelocity(RigidbodyComponent& rigidbody, Coords3f& angularForce, float ts) const;
	void _calculateExpectedVelocity(RigidbodyComponent& rigidbody, Coords3f& linearForce, float ts, const Coords3f& rotation) const;
	void _processSleepThreshold(RigidbodyComponent& rigidbody) const;
};


#endif // !__engine_math_physicsEngine__