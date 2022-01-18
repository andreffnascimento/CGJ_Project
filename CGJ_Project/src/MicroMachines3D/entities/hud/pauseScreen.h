#ifndef __mm3d_entities_hud_pauseScreen__
#define __mm3d_entities_hud_pauseScreen__


#include "MicroMachines3D/common/include.h"




class PauseScreen : public SceneEntity
{

public:
	PauseScreen(Scene* scene)
		: SceneEntity(scene->createEntity("PauseScreen"))
	{
		WindowCoords windowCoords = Application::getInstance().getOriginalWindowSize();
		CanvasComponent& canvas = addComponent<CanvasComponent>();

		createPauseBackground(scene, canvas, windowCoords);
		createPauseText(scene, canvas, windowCoords);
		createPauseIcon(scene, canvas, windowCoords);
	}




private:
	void createPauseBackground(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity pauseBackground = canvas.createImageEntity(scene, *this, "PauseBackground");
		ImageComponent& image = pauseBackground.getComponent<ImageComponent>();
		image.setBlendColor({ 0.15f, 0.15f, 0.15f, 0.5f });
		Transform::scaleTo(pauseBackground, { (float)windowCoords.x, (float)windowCoords.y, 1.0f });
		Transform::translateTo(pauseBackground, { windowCoords.x / 2.0f, windowCoords.y / 2.0f, 0.0f });
	}


	void createPauseText(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity pauseText = canvas.createTextEntity(scene, *this, "PauseText");
		TextComponent& text = pauseText.getComponent<TextComponent>();
		text.setText("Pause");
		text.setSize(1.5f);
		Transform::translateTo(pauseText, { windowCoords.x / 2.0f - 105.0f, windowCoords.y / 2.0f + 30.0f, 0.0f });
	}


	void createPauseIcon(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity pauseIcon = canvas.createImageEntity(scene, *this, "PauseIcon");
		ImageComponent& image = pauseIcon.getComponent<ImageComponent>();
		image.addImage(Renderer::create2dTexture("src/MicroMachines3D/textures/hud/pause.png"));
		image.setBlendColor({ 0.9f, 0.9f, 0.9f, 1.0f });
		Transform::scaleTo(pauseIcon, PAUSE_SIZE);
		Transform::translateTo(pauseIcon, { windowCoords.x / 2.0f, (windowCoords.y + PAUSE_SIZE.y) / 2.5f, 0.0f });
	}

};


#endif // !__mm3d_entities_hud_pauseScreen__