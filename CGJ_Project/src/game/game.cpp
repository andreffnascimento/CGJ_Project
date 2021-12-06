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




#include "game/gameInit.h"		// set of methods to create all the game objects and initialize the world

void Game::_init()
{
	initCameras(*this);
}