#ifndef __mm3d_scripts_manager_game_raceManagerScript__
#define __mm3d_scripts_manager_game_raceManagerScript__


#include "MicroMachines3D/common/include.h"




class RaceManagerScript : public Script
{
private:
	static constexpr float RESET_GAME_HEIGHT = -10.0f;


	

private:
	Entity _car = Entity();
	RigidbodyComponent* _carRigidbody = nullptr;

	bool _colliderWithOrange = false;



public:
	RaceManagerScript() = delete;
	RaceManagerScript(const RaceManagerScript&) = default;
	RaceManagerScript(Scene* scene) : Script(scene, "RaceManagerScript") {}
	~RaceManagerScript() = default;


public:
	void onCreate() override
	{
		_car = _scene->getEntityByTag("Car");
		_carRigidbody = &_car.getComponent<RigidbodyComponent>();
		_respawn();
	}


	void onUpdate(float ts) override
	{
		if (_colliderWithOrange || _carRigidbody->position().y < RESET_GAME_HEIGHT)
			_respawn();
	}




private:
	void _respawn()
	{
		Transform::translateTo(_car, { 0.0f, CAR_BOTTOM_SIZE.y / 2.0f + CAR_FLOOR_DISTANCE + CAR_SPAWN_HEIGHT, TABLE_SIZE.z / 3.0f });
		Transform::rotateTo(_car, { 0.0f, 90.0f, 0.0f });
		_carRigidbody->setVelocity(Coords3f({ 0.0f, 0.0f, 0.0f }));
	}

};


#endif // !__mm3d_scripts_manager_game_raceManagerScript__