#ifndef __mm3d_scripts_collisionResolvers_cheerioCollisionResolver__
#define __mm3d_scripts_collisionResolvers_cheerioCollisionResolver__


#include "MicroMachines3D/common/include.h"




class CheerioCollisionResolver : public CollisionResolver
{




public:
	CheerioCollisionResolver() = default;
	CheerioCollisionResolver(const CheerioCollisionResolver&) = default;
	CheerioCollisionResolver(AABBColliderComponent& collider) : CollisionResolver(collider) {}
	~CheerioCollisionResolver() = default;


public:
	void init() override
	{
		CollisionResolver::init();
		_setWhitelist(ColliderIds::CAR | ColliderIds::CHEERIO | ColliderIds::CANDEL | ColliderIds::TABLE);
	}

};


#endif // !__mm3d_scripts_collisionResolvers_cheerioCollisionResolver__