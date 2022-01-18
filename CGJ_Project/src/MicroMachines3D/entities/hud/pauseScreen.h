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
		Entity textEntity = canvas.createTextEntity(scene, *this, "PauseText");
		textEntity.getComponent<TextComponent>().setText("Pause");

		Entity imageEntity = canvas.createImageEntity(scene, *this, "HeartImage");
		imageEntity.getComponent<ImageComponent>().addImage(Renderer::create2dTexture("src/MicroMachines3D/textures/heart.png"));
		imageEntity.getComponent<ImageComponent>().setBlendColor({ 0.0f, 0.7f, 0.0f, 0.8f });
		Transform::scaleTo(imageEntity, { 50.0f, 50.0f, 1.0f });
		Transform::translateTo(imageEntity, { 1000.0f, 50.0f, 0.0f });
	}

};


#endif // !__mm3d_entities_hud_pauseScreen__