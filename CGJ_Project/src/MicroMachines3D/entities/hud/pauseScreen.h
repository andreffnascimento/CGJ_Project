#ifndef __mm3d_entities_hud_pauseScreen__
#define __mm3d_entities_hud_pauseScreen__


#include "MicroMachines3D/common/include.h"




class PauseScreen : public SceneEntity
{

public:
	PauseScreen(Scene* scene)
		: SceneEntity(scene->createEntity("PauseScreen"))
	{
		WindowCoords windowCoords = Application::getInstance().getWindowSize();

		CanvasComponent& canvas = addComponent<CanvasComponent>();

		createPauseText(scene, canvas, windowCoords);
	}




private:
	void createPauseText(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		canvas.addText(scene, *this, "Pause");
	}

};


#endif // !__mm3d_entities_hud_pauseScreen__