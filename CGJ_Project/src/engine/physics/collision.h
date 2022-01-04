#ifndef __engine_physics_collision
#define __engine_physics_collision


#include "engine/scene/components.h"
#include "engine/utils/coords.h"




struct Collision
{

private:
	const AABBColliderComponent& _collider;

	Coords3f _collisionNormal = Coords3f();
	Coords3f _relativeVelocity = Coords3f();
	float _impulse = 0.0f;




public:
	Collision() = delete;
	Collision(const Collision&) = default;
	Collision(const AABBColliderComponent& collider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse)
		: _collider(collider), _collisionNormal(collisionNormal), _relativeVelocity(relativeVelocity), _impulse(impulse) {};

	~Collision() = default;


public:
	inline const Coords3f& collisionNormal() const	{ return _collisionNormal; }
	inline const Coords3f& relativeVelocity() const	{ return _relativeVelocity; }
	inline float impulse()	const					{ return _impulse; }

};


#endif // !__engine_physics_collision__