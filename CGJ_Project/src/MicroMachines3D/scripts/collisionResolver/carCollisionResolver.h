#ifndef __mm3d_scripts_collisionResolvers_carCollisionResolver__
#define __mm3d_scripts_collisionResolvers_carCollisionResolver__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"
#include "MicroMachines3D/scripts/butters/buttersDriftScript.h"
#include "MicroMachines3D/scripts/boosters/boostersDriftScript.h"




class CarCollisionResolver : public CollisionResolver
{

private:
	const Scene* _scene;

	RaceManagerScript* _raceManagerScript = nullptr;
	ButtersDriftScript* _buttersDriftScript = nullptr;
	BoostersDriftScript* _boostersDriftScript = nullptr;

	unsigned int _nCheerioCollisions = 0;




public:
	CarCollisionResolver() = default;
	CarCollisionResolver(const CarCollisionResolver&) = default;
	CarCollisionResolver(const Scene* scene, AABBColliderComponent& collider) : CollisionResolver(collider), _scene(scene) {}
	~CarCollisionResolver() = default;


public:
	void init() override
	{
		CollisionResolver::init();
		_setBlacklist(0);	// car collides with everything

		_buttersDriftScript = dynamic_cast<ButtersDriftScript*>(_scene->getEntityByTag("Butters").getComponent<ScriptComponent>().getScriptByTag("ButtersDriftScript"));
		_boostersDriftScript = dynamic_cast<BoostersDriftScript*>(_scene->getEntityByTag("Boosters").getComponent<ScriptComponent>().getScriptByTag("BoostersDriftScript"));
		_raceManagerScript = dynamic_cast<RaceManagerScript*>(_scene->getEntityByTag("GameManager").getComponent<ScriptComponent>().getScriptByTag("RaceManagerScript"));
	}




protected:
	void _processCollision(const Collision& collision) override
	{
		switch (collision.collider().id())
		{

		case ColliderIds::CHEERIO:
			_nCheerioCollisions++;
			break;
		
		case ColliderIds::ORANGES:
			_raceManagerScript->setColliderWithOranges(true);
			return;

		case ColliderIds::BUTTER:
			_buttersDriftScript->addDriftingBody(_collider.rigidbody());
			return;

		case ColliderIds::BOOSTER:
			_boostersDriftScript->addDriftingBody(_collider.rigidbody());
			return;
		}

		CollisionResolver::_processCollision(collision);
	}

};


#endif // !__mm3d_scripts_collisionResolvers_carCollisionResolver__