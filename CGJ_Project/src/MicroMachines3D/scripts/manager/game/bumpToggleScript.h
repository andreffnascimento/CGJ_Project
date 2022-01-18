#ifndef __mm3d_scripts_manager_game_bumpToggleScript__
#define __mm3d_scripts_manager_game_bumpToggleScript__


#include "MicroMachines3D/common/include.h"




class BumpToggleScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	bool _bumpToggle = true;




public:
	BumpToggleScript() = delete;
	BumpToggleScript(const BumpToggleScript&) = default;
	BumpToggleScript(Scene* scene) : Script(scene, "BumpToggleScript") {}
	~BumpToggleScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
	}


	void onUpdate(float ts) override
	{
		if (_eventHandler->keyState('B').pressed() || _eventHandler->keyState('b').pressed())
		{
			_bumpToggle = !_bumpToggle;
			Renderer::setBumpActive(_bumpToggle);
		}
	}

};


#endif // !__mm3d_scripts_manager_game_bumpToggleScript__