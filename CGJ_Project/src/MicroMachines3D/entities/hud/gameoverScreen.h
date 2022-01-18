#ifndef __mm3d_entities_hud_gameoverScreen__
#define __mm3d_entities_hud_gameoverScreen__


#include "MicroMachines3D/common/include.h"




class GameoverScreen : public SceneEntity
{

public:
	GameoverScreen(Scene* scene)
		: SceneEntity(scene->createEntity("GameoverScreen"))
	{
		WindowCoords windowCoords = Application::getInstance().getOriginalWindowSize();
		CanvasComponent& canvas = addComponent<CanvasComponent>();
		canvas.setEnabled(true);

		createGameoverBackground(scene, canvas, windowCoords);
		createGameoverText(scene, canvas, windowCoords);
		createGameoverQuery(scene, canvas, windowCoords);
	}




private:
	void createGameoverBackground(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity gameoverBackground = canvas.createImageEntity(scene, *this, "GameoverBackground");
		ImageComponent& image = gameoverBackground.getComponent<ImageComponent>();
		image.setBlendColor({ 0.15f, 0.15f, 0.15f, 0.95f });
		Transform::scaleTo(gameoverBackground, { (float)windowCoords.x, (float)windowCoords.y, 1.0f });
		Transform::translateTo(gameoverBackground, { windowCoords.x / 2.0f, windowCoords.y / 2.0f, 0.0f });
	}


	void createGameoverText(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity gameoverText = canvas.createTextEntity(scene, *this, "GameoverText");
		TextComponent& text = gameoverText.getComponent<TextComponent>();
		text.setText("You lost!");
		text.setSize(1.5f);
		Transform::translateTo(gameoverText, { windowCoords.x / 2.0f - 140.0f, windowCoords.y / 2.0f, 0.0f });
	}


	void createGameoverQuery(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity gameoverQuery = canvas.createTextEntity(scene, *this, "GameoverQuery");
		TextComponent& text = gameoverQuery.getComponent<TextComponent>();
		text.setText("Press \'R\' to start a new race");
		text.setSize(0.8f);
		Transform::translateTo(gameoverQuery, { windowCoords.x / 2.0f - 240.0f, windowCoords.y / 2.0f - 50.0f, 0.0f });
	}

};


#endif // !__mm3d_entities_hud_gameoverScreen__