#ifndef __mm3d_scripts_manager_game_fogToggleScript__
#define __mm3d_scripts_manager_game_fogToggleScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"




class FogToggleScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	const RaceManagerScript* _raceManagerScript = nullptr;

	bool _fogToggle = true;




public:
	FogToggleScript() = delete;
	FogToggleScript(const FogToggleScript&) = default;
	FogToggleScript(Scene* scene) : Script(scene, "FogToggleScript") {}
	~FogToggleScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_raceManagerScript = dynamic_cast<RaceManagerScript*>(_scene->getEntityByTag("GameManager").getComponent<ScriptComponent>().getScriptByTag("RaceManagerScript"));
	}


	void onUpdate(float ts) override
	{
		if (_raceManagerScript->paused())
			return;

		if (_eventHandler->keyState('F').pressed() || _eventHandler->keyState('f').pressed())
		{
			_fogToggle = !_fogToggle;
			Renderer::setFogActive(_fogToggle);
		}
	}

};


#endif // !__mm3d_scripts_manager_game_fogToggleScript__