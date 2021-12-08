#ifndef __ENGINE_APP_CALLBACK_H__
#define __ENGINE_APP_CALLBACK_H__


#include <iostream>
#include <sstream>

#include "engine/app/application.h"
#include "engine/math/AVTmathLib.h"

#include <GL/freeglut.h>




void terminateApp()
{
	Application::terminate();
}


// FPS display callback function
void timer(int value)
{
	Application &app = Application::getInstance();
	if (!app._running)
		return;

	const Application::ApplicationData &appData = app._applicationData;
	app._frameCount = 0;

	std::ostringstream oss;
	oss << appData.caption << ": " << app._frameCount << " FPS @ (" << appData.width << "x" << appData.heigth << ")";
	std::string s = oss.str();
	glutSetWindow(app._windowHandle);
	glutSetWindowTitle(s.c_str());
	glutTimerFunc(1000, timer, 0);
}


// Screen refresh callback function
void refresh(int value)
{
	Application& app = Application::getInstance();
	if (!app._running)
		return;

	glutTimerFunc((unsigned int)(1000.0 / 60.0), refresh, 0);
	glutPostRedisplay();
}


// Reshape callback Function
void changeViewportSize(int width, int height)
{
	Application &app = Application::getInstance();
	if (app._scene != nullptr && app._scene->getActiveCamera() != nullptr)
		app._scene->getActiveCamera()->setViewport(width, height);
}


// Render callback function
void displayScene()
{
	Application &app = Application::getInstance();
	if (app._scene == nullptr)
		throw std::string("Application is missing an attached scene!");

	app._frameCount++;
	app._scene->update();
	app._renderer->renderScene(*app._scene);	
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


#endif // !__ENGINE_APP_CALLBACK_H__