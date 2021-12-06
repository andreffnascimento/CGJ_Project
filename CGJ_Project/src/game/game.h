#ifndef GAME_GAME_H
#define GAME_GAME_H


#include <list>

#include "game/gameObject.h"
#include "game/camera.h"
#include "math/AVTmathLib.h"



class Game
{

private:
	std::list<GameObject*> _gameObjects;	// all the objects of the world that are being rendered
	Camera *_activeCamera;					// the current camera being used




public:
	Game();
	~Game();


public:
	void update();


public:
	inline void setActiveCamera(Camera *camera) { _activeCamera = camera; }


private:
	void _init();

};


#endif // !GAME_GAME_H