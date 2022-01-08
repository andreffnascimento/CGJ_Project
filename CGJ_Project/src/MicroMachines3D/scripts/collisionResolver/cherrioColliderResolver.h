#ifndef __mm3d_scripts_collisionResolvers_cheerioColliderResolver__
#define __mm3d_scripts_collisionResolvers_cheerioColliderResolver__


#include "MicroMachines3D/common/include.h"




class CheerioCollisionResolver : public CollisionResolver
{

private:
	const Scene* _scene;




public:
	CheerioCollisionResolver() = default;
	CheerioCollisionResolver(const CheerioCollisionResolver&) = default;
	CheerioCollisionResolver(const Scene* scene, AABBColliderComponent& collider) : CollisionResolver(collider), _scene(scene) {}
	~CheerioCollisionResolver() = default;


public:
	void init() override
	{
		CollisionResolver::init();
		_setWhitelist({ ColliderIds::CAR, ColliderIds::CHEERIO });
	}




protected:
	void _processCollision(const Collision& collision)
	{
		CollisionResolver::_processCollision(collision);
	}

};


#endif // !__mm3d_scripts_collisionResolvers_cheerioColliderResolver__