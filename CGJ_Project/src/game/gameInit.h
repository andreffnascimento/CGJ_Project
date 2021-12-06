#ifndef GAME_GAME_INIT_H
#define GAME_GAME_INIT_H


#include "app/application.h"
#include "gameObjects/camera/camera.h"
#include "scripts/camera/worldCameraScript.h"



void initCameras(Game &game)
{
	Camera *camera1 = new Camera();
	const InputHandler &input = Application::getInputHandler();
	camera1->addScript(new WorldCameraScript(*camera1, input));
	game.addGameObject(camera1);

	game.setActiveCamera(camera1);
}


#endif // !GAME_GAME_INIT_H
