#ifndef __mm3d_scripts_candels_candelsLightScript__
#define __mm3d_scripts_candels_candelsLightScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"

#include <list>




class CandelsLightScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	const RaceManagerScript* _raceManagerScript = nullptr;

	std::list<LightComponent*> _lights = std::list<LightComponent*>();

	bool _candelsOn = false;




public:
	CandelsLightScript() = delete;
	CandelsLightScript(const CandelsLightScript&) = default;
	CandelsLightScript(Scene* scene) : Script(scene, "CandelsLightScript") {}
	~CandelsLightScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_raceManagerScript = dynamic_cast<RaceManagerScript*>(_scene->getEntityByTag("GameManager").getComponent<ScriptComponent>().getScriptByTag("RaceManagerScript"));
		for (auto& candel : _scene->getEntitiesByTag(std::regex("^Candels:light_.*$")))
			_lights.push_back(&candel.getComponent<LightComponent>());
	}


	void onUpdate(float ts) override
	{
		if (!_raceManagerScript->playing())
			return;

		if (_eventHandler->keyState('C').pressed() || _eventHandler->keyState('c').pressed())
		{
			_candelsOn = !_candelsOn;
			for (auto& light : _lights)
				light->setEnabled(_candelsOn);
		}
	}

};


#endif // !__mm3d_scripts_candels_candelsLightScript__