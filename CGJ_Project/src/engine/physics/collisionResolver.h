#ifndef __engine_physics_collisionResolver__
#define __engine_physics_collisionResolver__


#include <list>

#include "engine/physics/collision.h"




class CollisionResolver
{

private:
	std::list<Collision> _collisions = std::list<Collision>();




public:
	CollisionResolver() = default;
	CollisionResolver(const CollisionResolver&) = default;
	~CollisionResolver() = default;


public:

};


#endif // !__engine_physics_collisionResolver__