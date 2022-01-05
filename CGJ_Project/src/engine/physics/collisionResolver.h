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
	AABBColliderComponent& _collider; 

	std::list<Collision> _collisions = std::list<Collision>();
	std::list<Force> _impulseForces = std::list<Force>();




public:
	CollisionResolver() = delete;
	CollisionResolver(const CollisionResolver&) = default;
	CollisionResolver(AABBColliderComponent& collider);
	~CollisionResolver() = default;


public:
	inline const std::list<Collision>& collisions() const	{ return _collisions; }
	inline bool collided() const							{ return !_collisions.empty(); }
	inline bool updated() const								{ return _impulseForces.empty(); }


public:
	void reset();
	void addCollision(const AABBColliderComponent& otherCollider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse);
	void processCollisions();
	void updateVelocity(Coords3f& velocity) const;


public:
	virtual bool ignoreCollision(const RigidbodyComponent& rigidbody);


protected:
	virtual void _processCollision(const Collision& collision);

};


#endif // !__engine_physics_collisionResolver__