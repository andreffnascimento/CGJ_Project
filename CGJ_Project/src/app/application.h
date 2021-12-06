#ifndef APP_APPLICATION_H
#define APP_APPLICATION_H

#include <iostream>

#include "app/inputHandler.h"
#include "game/game.h"
#include "math/AVTmathLib.h"
#include "renderer/shaderHelper.h"
#include "text/avtFreeType.h"

#include <GL/glew.h>		// include GLEW to access OpenGL 3.3 functions
#include <GL/freeglut.h>	// GLUT is the toolkit to interface with the OS
#include <IL/il.h>			// DevIL library



class Application
{

public:
	static const char *CAPTION;
	static const char *FONT_NAME;
	static const unsigned int WIDTH = 1024;
	static const unsigned int HEIGHT = 768;



private:
	int _windowHandle;
	unsigned int _frameCount;

	InputHandler _inputHandler;
	Game *_game;



private:
	Application();		// application is a singleton


public:
	~Application();
	

public:
	static Application& getInstance();
	static InputHandler& getInputHandler();


public:
	void init(int argc, char** argv, bool lockedFps);


public:
	void run();


public:
	inline void updateGame() { _game->update(); }


public:
	friend void timer(int value);
	friend void renderScene();
};


#endif // !APP_APPLICATION_H