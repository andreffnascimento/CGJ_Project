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




public:
	PhysicsEngine() = default;
	PhysicsEngine(const PhysicsEngine&) = default;
	~PhysicsEngine() = default;


public:
	void run(const Scene& scene, float ts) const;




private:
	void _processRigidbody(EntityHandle entityId, RigidbodyComponent& rigidbody, float ts) const;


private:
	void _calculateExpectedVelocity(RigidbodyComponent& rigidbody, float ts) const;
	Coords3f _calculateDragForce(const RigidbodyComponent& rigidbody) const;

};


#endif // !__engine_math_physicsEngine__
