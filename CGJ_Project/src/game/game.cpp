#include "game.h"



Game::Game()
	: _gameObjects(std::list<GameObject*>()), _activeCamera(nullptr)
{
	_init();
}


Game::~Game()
{
	for (auto& gameObject : _gameObjects)
		delete gameObject;
}





void Game::update()
{
	// updates all the game objects
	for (auto& gameObject : _gameObjects)
		gameObject->update();

	// updates the active camera
	if (_activeCamera != nullptr)
	{
		const Camera::Coords &cameraCoords = _activeCamera->getCoords();
		lookAt(cameraCoords.x, cameraCoords.y, cameraCoords.z, 0, 0, 0, 0, 1, 0);
	}
}




#include "app/application.h"
#include "scripts/camera/worldCameraScript.h"

void Game::_init()
{
	InputHandler &input = Application::getInputHandler();
	Camera *camera1 = new Camera();
	camera1->addScript(new WorldCameraScript(*camera1, Application::getInputHandler()));
	_gameObjects.push_back(camera1);

	_activeCamera = camera1;
}
