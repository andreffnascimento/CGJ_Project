#ifndef __mm3d_scripts_car_carHeadlightsScript__
#define __mm3d_scripts_car_carHeadlightsScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"




class CarHeadlightsScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	const RaceManagerScript* _raceManagerScript = nullptr;

	MeshComponent* _frontMesh = nullptr;
	MeshComponent* _backMesh  = nullptr;

	LightComponent* _leftFrontLight = nullptr;
	LightComponent* _rightFrontLight = nullptr;

	bool _lightsOn = false;




public:
	CarHeadlightsScript() = delete;
	CarHeadlightsScript(const CarHeadlightsScript&) = default;
	CarHeadlightsScript(Scene* scene) : Script(scene, "CarHeadlightsScript") {}
	~CarHeadlightsScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_raceManagerScript = dynamic_cast<RaceManagerScript*>(_scene->getEntityByTag("GameManager").getComponent<ScriptComponent>().getScriptByTag("RaceManagerScript"));
		_frontMesh = &_scene->getEntityByTag("Car:headlight-frontLeft").getComponent<MeshComponent>();
		_backMesh  = &_scene->getEntityByTag("Car:headlight-backLeft").getComponent<MeshComponent>();
		_leftFrontLight = &_scene->getEntityByTag("Car:headlight-frontLeft").getComponent<LightComponent>();
		_rightFrontLight = &_scene->getEntityByTag("Car:headlight-frontRight").getComponent<LightComponent>();
	}


	void onUpdate(float ts) override
	{
		if (_raceManagerScript->paused())
			return;

		if (_eventHandler->keyState('H').pressed() || _eventHandler->keyState('h').pressed())
		{
			_lightsOn = !_lightsOn;

			if (_lightsOn)
			{
				_frontMesh->setMaterial(CAR_HEADLIGHT_FRONT_ON_MATERIAL);
				_backMesh->setMaterial(CAR_HEADLIGHT_BACK_ON_MATERIAL);
				_leftFrontLight->setEnabled(true);
				_rightFrontLight->setEnabled(true);
			}
			else
			{
				_frontMesh->setMaterial(CAR_HEADLIGHT_FRONT_OFF_MATERIAL);
				_backMesh->setMaterial(CAR_HEADLIGHT_BACK_OFF_MATERIAL);
				_leftFrontLight->setEnabled(false);
				_rightFrontLight->setEnabled(false);
			}
		}
	}

};


#endif // !__mm3d_scripts_car_carHeadlightsScript__
