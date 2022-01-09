#ifndef __mm3d_scripts_collisionResolvers_carCollisionResolver__
#define __mm3d_scripts_collisionResolvers_carCollisionResolver__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/butters/buttersDriftScript.h"




class CarCollisionResolver : public CollisionResolver
{

private:
	const Scene* _scene;

	ButtersDriftScript* buttersDriftScript = nullptr;

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

		buttersDriftScript = dynamic_cast<ButtersDriftScript*>(_scene->getEntityByTag("Butters").getComponent<ScriptComponent>().getScriptByTag("ButtersDriftScript"));
	}




protected:
	void _initCollisionProcessing() override
	{
		_nCheerioCollisions = 0;
	}


	void _processCollision(const Collision& collision) override
	{
		if (collision.collider().id() == ColliderIds::CHEERIO)
		{
			_nCheerioCollisions++;
		}

		else if (collision.collider().id() == ColliderIds::BUTTER)
		{
			buttersDriftScript->addDriftingBody(_collider.rigidbody());
			return;
		}

		CollisionResolver::_processCollision(collision);
	}


	void _terminateVelocityProcessing(Coords3f& velocity) override
	{
		if (_nCheerioCollisions == 0)
			return;

		velocity.x /= _nCheerioCollisions;
		velocity.z /= _nCheerioCollisions;
	}

};


#endif // !__mm3d_scripts_collisionResolvers_carCollisionResolver__