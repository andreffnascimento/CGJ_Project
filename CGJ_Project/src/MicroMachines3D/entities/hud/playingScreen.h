#ifndef __mm3d_entities_hud_playingScreen__
#define __mm3d_entities_hud_playingScreen__


#include "MicroMachines3D/common/include.h"




class PlayingScreen : public SceneEntity
{

public:
	PlayingScreen(Scene* scene)
		: SceneEntity(scene->createEntity("PlayingScreen"))
	{
		WindowCoords windowCoords = Application::getInstance().getOriginalWindowSize();
		CanvasComponent& canvas = addComponent<CanvasComponent>();
		canvas.setEnabled(true);

		createHeartIcon(scene, canvas, windowCoords, 0);
		createHeartIcon(scene, canvas, windowCoords, 1);
		createHeartIcon(scene, canvas, windowCoords, 2);
		createHeartIcon(scene, canvas, windowCoords, 3);
		createHeartIcon(scene, canvas, windowCoords, 4);
		createGasPedalIcon(scene, canvas, windowCoords);
		createBrakePedalIcon(scene, canvas, windowCoords);
		createResetQuery(scene, canvas, windowCoords);
	}




private:
	void createHeartIcon(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords, unsigned int id)
	{
		Entity heart = canvas.createImageEntity(scene, *this, "Heart_" + std::to_string(id));
		ImageComponent& image = heart.getComponent<ImageComponent>();
		image.addImage(Renderer::create2dTexture("src/MicroMachines3D/textures/hud/heart.png"));

		float xHeartShift = HEART_SIZE.x * 1.2f * id;
		Transform::scaleTo(heart, HEART_SIZE);
		Transform::translateTo(heart, { HEART_SIZE.x / 1.8f + xHeartShift, windowCoords.y - HEART_SIZE.y / 1.8f, 0.0f });
	}


	void createGasPedalIcon(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity gasPedal = canvas.createImageEntity(scene, *this, "GasPedal");
		ImageComponent& image = gasPedal.getComponent<ImageComponent>();
		image.addImage(Renderer::create2dTexture("src/MicroMachines3D/textures/hud/gasPedal.png"));

		Transform::scaleTo(gasPedal, GASPEDAL_SIZE);
		Transform::translateTo(gasPedal, { windowCoords.x - 2.0f * GASPEDAL_SIZE.x / 1.8f, GASPEDAL_SIZE.y / 1.8f, 0.0f });
	}


	void createBrakePedalIcon(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity brakePedal = canvas.createImageEntity(scene, *this, "BrakePedal");
		ImageComponent& image = brakePedal.getComponent<ImageComponent>();
		image.addImage(Renderer::create2dTexture("src/MicroMachines3D/textures/hud/brakePedal.png"));

		Transform::scaleTo(brakePedal, GASPEDAL_SIZE);
		Transform::translateTo(brakePedal, { windowCoords.x - 4.0f * GASPEDAL_SIZE.x / 1.8f, GASPEDAL_SIZE.y / 1.8f, 0.0f });
	}


	void createResetQuery(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords)
	{
		Entity resetQuery = canvas.createTextEntity(scene, *this, "ResetQuery");
		TextComponent& text = resetQuery.getComponent<TextComponent>();
		text.setText("Press \'R\' again to reset the race!");
		text.setSize(0.7f);
		Transform::translateTo(resetQuery, { windowCoords.x / 2.0f + 100.0f, windowCoords.y - 40.0f, 0.0f});
	}

};


#endif // !__mm3d_entities_hud_playingScreen__