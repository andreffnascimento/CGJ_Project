#ifndef APP_CALLBACK_H
#define APP_CALLBACK_H


#include <iostream>
#include <sstream>

#include "app/application.h"
#include "app/test.h"



// FPS display callback function
void timer(int value)
{
	Application& app = Application::getInstance();
	std::ostringstream oss;
	oss << Application::CAPTION << ": " << app._frameCount << " FPS @ (" << Application::WIDTH << "x" << Application::HEIGHT << ")";
	std::string s = oss.str();
	glutSetWindow(app._windowHandle);
	glutSetWindowTitle(s.c_str());
	glutTimerFunc(1000, timer, 0);
	app._frameCount = 0;
}


// Screen refresh callback function
void refresh(int value)
{
	glutTimerFunc(1000.0 / 60.0, refresh, 0);
	glutPostRedisplay();
}


// Reshape callback Function
void changeSize(int width, int height) {

	float ratio;
	if (height == 0)					// prevent a divide by zero, when window is too short
		height = 1;

	glViewport(0, 0, width, height);	// set the viewport to be the entire window

	// set the projection matrix
	ratio = (1.0f * width) / height;
	loadIdentity(PROJECTION);
	perspective(53.13f, ratio, 0.1f, 1000.0f);
}


// Render callback function
void renderScene()
{
	Application &app = Application::getInstance();
	app._frameCount++;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);

	// use our shader
	glUseProgram(getShader().getProgramIndex());

	app.updateGame();

	renderLight();		// TEMP
	renderTable();		// TEMP
	renderText();		// TEMP
	
	app._inputHandler.reset();
	glutSwapBuffers();
}




// Keyboard callback function
void processKeys(unsigned char key, int x, int y)
{
	InputHandler &input = Application::getInputHandler();
	input.pressKey(key);
}


// Mouse click callback function
void processMouseButtons(int button, int state, int x, int y)
{
	InputHandler &input = Application::getInputHandler();
	InputHandler::MouseStatus status = InputHandler::MouseStatus::NONE;

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
			status = InputHandler::MouseStatus::LEFT_DOWN;
		else if (button == GLUT_RIGHT_BUTTON)
			status = InputHandler::MouseStatus::RIGHT_DOWN;
	}
	else if (state == GLUT_UP)
		status = InputHandler::MouseStatus::MOUSE_UP;
	
	input.updateMouse(x, y);
	input.updateMouseStatus(status);
}


// Mouse motion callback function
void processMouseMotion(int x, int y)
{
	InputHandler &input = Application::getInputHandler();
	input.updateMouse(x, y);
	input.updateMouseStatus(InputHandler::MouseStatus::MOVE);
}


// Mouse wheel callback function
void mouseWheel(int wheel, int direction, int x, int y) 
{
	InputHandler &input = Application::getInputHandler();
	input.updateMouse(x, y);
	input.updateMouseWeel(direction);
	input.updateMouseStatus(InputHandler::MouseStatus::SCROL);
}


#endif // !APP_CALLBACK_H