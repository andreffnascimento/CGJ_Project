#ifndef GAME_GAME_INIT_H
#define GAME_GAME_INIT_H


#include "app/application.h"
#include "scripts/camera/worldCameraScript.h"



void initCameras(Game &game)
{
	InputHandler& input = Application::getInputHandler();
	Camera* camera1 = new Camera();
	camera1->addScript(new WorldCameraScript(*camera1, Application::getInputHandler()));
	game.addGameObject(camera1);

	game.setActiveCamera(camera1);
}


#endif // !GAME_GAME_INIT_H
