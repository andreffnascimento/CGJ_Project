#ifndef __engine_math_physicsEngine__
#define __engine_math_physicsEngine__


#include "engine/scene/entity.h"
#include "engine/scene/components.h"


class Scene;




class PhysicsEngine
{

public:
	static constexpr float GRAVITY = 9.8f;
	



private:
	




public:
	PhysicsEngine() = default;
	PhysicsEngine(const PhysicsEngine&) = default;
	~PhysicsEngine() = default;


public:
	void run(const Scene& scene) const;




private:
	void processRigidbody(EntityHandle entityId, RigidbodyComponent& rigidbody) const;


private:
	void combineForces(RigidbodyComponent& rigidbody, Coords3f& linearForce, Coords3f& angularForce) const;
	void addGravityForce(const RigidbodyComponent& rigidbody, Coords3f& linearForce) const;

};


#endif // !__engine_math_physicsEngine__
