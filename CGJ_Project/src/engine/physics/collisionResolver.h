#ifndef __engine_physics_collisionResolver__
#define __engine_physics_collisionResolver__


#include <list>

#include "engine/physics/collision.h"
#include "engine/physics/force.h"

#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class CollisionResolver
{

protected:
	std::list<Collision> _collisions = std::list<Collision>();
	std::list<Force> _impulseForces = std::list<Force>();




public:
	CollisionResolver() = default;
	CollisionResolver(const CollisionResolver&) = default;
	~CollisionResolver() = default;


public:
	inline const std::list<Collision>& collisions() const	{ return _collisions; }
	inline bool collided() const							{ return !_collisions.empty(); }
	inline bool updated() const								{ return _impulseForces.empty(); }


public:
	void reset();
	void addCollision(const AABBColliderComponent& otherCollider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse);
	void processCollisions(const AABBColliderComponent& collider);
	void updateVelocity(Coords3f& velocity);


public:
	virtual bool ignoreCollision(const RigidbodyComponent& rigidbody);


protected:
	virtual void _processCollision(const AABBColliderComponent& collider, const Collision& collision);




public:
	static void defaultCollisionResolver(const AABBColliderComponent& collider, const Collision& collision);

};


#endif // !__engine_physics_collisionResolver__