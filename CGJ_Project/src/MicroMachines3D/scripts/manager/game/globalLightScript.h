#ifndef __mm3d_scripts_manager_game_globaLightScript__
#define __mm3d_scripts_manager_game_globaLightScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"





class GlobalLightScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	const RaceManagerScript* _raceManagerScript = nullptr;

	LightComponent* _globalLight = nullptr;




public:
	GlobalLightScript() = delete;
	GlobalLightScript(const GlobalLightScript&) = default;
	GlobalLightScript(Scene* scene) : Script(scene, "GlobalLightScript") {}
	~GlobalLightScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_raceManagerScript = dynamic_cast<RaceManagerScript*>(_scene->getEntityByTag("GameManager").getComponent<ScriptComponent>().getScriptByTag("RaceManagerScript"));
		_globalLight = &_scene->getEntityByTag("GlobalLight").getComponent<LightComponent>();
	}


	void onUpdate(float ts) override
	{
		if (!_raceManagerScript->playing())
			return;

		if (_eventHandler->keyState('N').pressed() || _eventHandler->keyState('n').pressed())
			_globalLight->setEnabled(!_globalLight->isEnabled());
	}

};


#endif // !__mm3d_scripts_manager_game_globaLightScript__