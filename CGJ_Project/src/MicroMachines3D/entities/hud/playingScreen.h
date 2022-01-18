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
	}




private:
	void createHeartIcon(Scene* scene, CanvasComponent& canvas, const WindowCoords& windowCoords, unsigned int id)
	{
		Entity imageEntity = canvas.createImageEntity(scene, *this, "Heart_" + std::to_string(id));
		ImageComponent& image = imageEntity.getComponent<ImageComponent>();
		image.addImage(Renderer::create2dTexture("src/MicroMachines3D/textures/hud/heart.png"));

		float xHeartShift = HEART_SIZE.x * 1.2f * id;
		Transform::scaleTo(imageEntity, HEART_SIZE);
		Transform::translateTo(imageEntity, { HEART_SIZE.x / 1.8f + xHeartShift, windowCoords.y - HEART_SIZE.y / 1.8f, 0.0f });
	}

};


#endif // !__mm3d_entities_hud_playingScreen__