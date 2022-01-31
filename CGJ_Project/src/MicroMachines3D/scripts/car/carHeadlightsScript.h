#ifndef __mm3d_scripts_car_carHeadlightsScript__
#define __mm3d_scripts_car_carHeadlightsScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"




class CarHeadlightsScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	const RaceManagerScript* _raceManagerScript = nullptr;

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
		_leftFrontLight = &_scene->getEntityByTag("Car:headlight-frontLeft").getComponent<LightComponent>();
		_rightFrontLight = &_scene->getEntityByTag("Car:headlight-frontRight").getComponent<LightComponent>();
	}


	void onUpdate(float ts) override
	{
		if (!_raceManagerScript->playing())
			return;

		if (_eventHandler->keyState('H').pressed() || _eventHandler->keyState('h').pressed())
		{
			_lightsOn = !_lightsOn;

			if (_lightsOn)
			{
				_leftFrontLight->setEnabled(true);
				_rightFrontLight->setEnabled(true);
			}
			else
			{
				_leftFrontLight->setEnabled(false);
				_rightFrontLight->setEnabled(false);
			}
		}
	}

};


#endif // !__mm3d_scripts_car_carHeadlightsScript__
