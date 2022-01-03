#ifndef __engine_physics_collision
#define __engine_physics_collision


#include "engine/scene/components.h"
#include "engine/utils/coords.h"




struct Collision
{

private:
	const AABBColliderComponent& _collider;

	Coords3f _relativeVelocity = Coords3f();
	Coords3f _impulseForce = Coords3f();




public:
	Collision() = delete;
	Collision(const Collision&) = default;
	Collision(AABBColliderComponent& collider) : _collider(collider) {};
	~Collision() = default;


public:
	inline const Coords3f& relativeVelocity()	{ return _relativeVelocity; }
	inline const Coords3f& impulseForce()		{ return _impulseForce; }

	inline void setRelativeVelocity(const Coords3f& relativeVelocity)	{ _relativeVelocity = relativeVelocity; }
	inline void setImpulseForce(const Coords3f& impulseForce)			{ _impulseForce = impulseForce; }

};


#endif // !__engine_physics_collision__