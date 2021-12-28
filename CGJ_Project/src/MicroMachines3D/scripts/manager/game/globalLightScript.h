#ifndef __mm3d_scripts_manager_game_globaLightScript__
#define __mm3d_scripts_manager_game_globaLightScript__


#include "MicroMachines3D/common/include.h"




class GlobalLightScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	LightComponent* _globalLight = nullptr;




public:
	GlobalLightScript() = delete;
	GlobalLightScript(const GlobalLightScript&) = default;
	GlobalLightScript(Scene* scene) : Script(scene) {}
	~GlobalLightScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_globalLight = &_scene->getEntityByTag("GlobalLight").getComponent<LightComponent>();
	}


	void onUpdate(float ts) override
	{
		if (_eventHandler->keyState('N').pressed() || _eventHandler->keyState('n').pressed())
			_globalLight->setEnabled(!_globalLight->isEnabled());
	}

};


#endif // !__mm3d_scripts_manager_game_globaLightScript__