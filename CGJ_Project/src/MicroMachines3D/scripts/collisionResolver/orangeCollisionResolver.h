#ifndef __mm3d_scripts_collisionResolvers_orangeCollisionResolver__
#define __mm3d_scripts_collisionResolvers_orangeCollisionResolver__


#include "MicroMachines3D/common/include.h"




class OrangeCollisionResolver : public CollisionResolver
{

public:
	OrangeCollisionResolver() = default;
	OrangeCollisionResolver(const OrangeCollisionResolver&) = default;
	OrangeCollisionResolver(AABBColliderComponent& collider) : CollisionResolver(collider) {}
	~OrangeCollisionResolver() = default;


public:
	void init() override
	{
		CollisionResolver::init();
		_setWhitelist(ColliderIds::CAR);
	}




protected:
	void _processCollision(const Collision& collision)
	{
		// empty
	}

};


#endif // !__mm3d_scripts_collisionResolvers_orangeCollisionResolver__