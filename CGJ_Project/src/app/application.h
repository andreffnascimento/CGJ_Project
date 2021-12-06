#ifndef APP_APPLICATION_H
#define APP_APPLICATION_H

#include <iostream>


#include "inputHandler.h"


// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "math/AVTmathLib.h"



class Application
{

public:
	static const char *CAPTION;
	static const unsigned int WIDTH = 1024;
	static const unsigned int HEIGHT = 768;


private:
	InputHandler _inputHandler;

	int _windowHandle = 0;
	unsigned int _frameCount = 0;





private:
	Application();		// application is a singleton


public:
	static Application& getInstance();
	static InputHandler& getInputHandler();


public:
	void init(int argc, char** argv, bool lockedFps);



public:
	friend void timer(int value);
};


#endif // !APP_APPLICATION_H