#ifndef __mm3d_scripts_manager_game_raceManagerScript__
#define __mm3d_scripts_manager_game_raceManagerScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/car/carMovementScript.h"
#include "MicroMachines3D/scripts/oranges/orangesManagerScript.h"




class RaceManagerScript : public Script
{
private:
	static constexpr float RESET_GAME_HEIGHT = -10.0f;


	

private:
	Entity _car = Entity();
	RigidbodyComponent* _carRigidbody = nullptr;

	CarMovementScript* _carMovementScript = nullptr;
	OrangesManagerScript* _orangesManagerScript = nullptr;

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
		_carMovementScript = dynamic_cast<CarMovementScript*>(_scene->getEntityByTag("Car").getComponent<ScriptComponent>().getScriptByTag("CarMovementScript"));
		_orangesManagerScript = dynamic_cast<OrangesManagerScript*>(_scene->getEntityByTag("Oranges").getComponent<ScriptComponent>().getScriptByTag("OrangesManagerScript"));
		_respawn();
	}


	void onUpdate(float ts) override
	{
		if (_colliderWithOrange || _carRigidbody->position().y < RESET_GAME_HEIGHT)
			_respawn();
	}




public:
	inline void setColliderWithOranges(bool colliderWithOranges) { _colliderWithOrange = colliderWithOranges; }




private:
	void _respawn()
	{
		Transform::translateTo(_car, Coords3f({ 0.0f, CAR_BOTTOM_SIZE.y / 2.0f + CAR_FLOOR_DISTANCE + CAR_SPAWN_HEIGHT, TABLE_SIZE.z / 3.0f }));
		Transform::rotateTo(_car, Coords3f({ 0.0f, -90.0f, 0.0f }));
		_carRigidbody->setVelocity(Coords3f({ 0.0f, 0.0f, 0.0f }));
		_carRigidbody->setAngularVelocity(Coords3f({ 0.0f, 0.0f, 0.0f }));
		_carMovementScript->reset();
		_orangesManagerScript->resetOrangesSpeed();
		_colliderWithOrange = false;
	}

};


#endif // !__mm3d_scripts_manager_game_raceManagerScript__