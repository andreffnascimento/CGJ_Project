#ifndef __mm3d_scripts_manager_game_fogToggleScript__
#define __mm3d_scripts_manager_game_fogToggleScript__


#include "MicroMachines3D/common/include.h"




class FogToggleScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	bool _fogToggle = false;




public:
	FogToggleScript() = delete;
	FogToggleScript(const FogToggleScript&) = default;
	FogToggleScript(Scene* scene) : Script(scene, "FogToggleScript") {}
	~FogToggleScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
	}


	void onUpdate(float ts) override
	{
		if (_eventHandler->keyState('F').pressed() || _eventHandler->keyState('f').pressed())
		{
			_fogToggle = !_fogToggle;
			Renderer::setFogActive(_fogToggle);
		}
	}

};


#endif // !__mm3d_scripts_manager_game_fogToggleScript__