#ifndef __engine_math_physicsEngine__
#define __engine_math_physicsEngine__


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
	PhysicsEngine() = default;
	PhysicsEngine(const PhysicsEngine&) = default;
	~PhysicsEngine() = default;


public:
	void run(const Scene& scene, float ts) const;




private:
	void _processRigidbody(const Scene& scene, EntityHandle entityId, RigidbodyComponent& rigidbody, float ts) const;


private:
	Coords3f _calculateDragForce(const RigidbodyComponent& rigidbody) const;
	void _calculateExpectedVelocity(RigidbodyComponent& rigidbody, float ts) const;
	void _processVelocityConstraints(RigidbodyComponent& rigidbody) const;
	void _processSleepThreshold(RigidbodyComponent& rigidbody) const;

};


#endif // !__engine_math_physicsEngine__