#ifndef __mm3d_scripts_collisionResolvers_carCollisionResolver__
#define __mm3d_scripts_collisionResolvers_carCollisionResolver__


#include "MicroMachines3D/common/include.h"




class CarCollisionResolver : public CollisionResolver
{

private:
	const Scene* _scene;




public:
	CarCollisionResolver() = default;
	CarCollisionResolver(const CarCollisionResolver&) = default;
	CarCollisionResolver(const Scene* scene, AABBColliderComponent& collider) : CollisionResolver(collider), _scene(scene) {}
	~CarCollisionResolver() = default;


public:
	void init() override
	{
		CollisionResolver::init();
		_setBlacklist({  });	// car collides with everything
	}




protected:
	void _processCollision(const Collision& collision)
	{
		if (collision.collider().id() == ColliderIds::BUTTER)
			return;

		CollisionResolver::_processCollision(collision);
	}

};


#endif // !__mm3d_scripts_collisionResolvers_carCollisionResolver__