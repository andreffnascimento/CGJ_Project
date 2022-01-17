#ifndef __engine_app_callback__
#define __engine_app_callback__


#include <iostream>
#include <sstream>

#include "engine/app/application.h"

#include <GL/freeglut.h>




// Application termination callback function
void terminateApp()
{
	Application::terminate();
}


// Overlay refesh timer callback function
void overlayTimer(int value)
{
	if (!Application::isRunning())
		return;
	
	glutTimerFunc(1000, overlayTimer, 0);
	displayOverlay();
}


// Screen refresh timer callback function
void refreshTimer(int value)
{
	if (!Application::isRunning())
		return;

	glutTimerFunc((unsigned int)(1000.0 / 60.0), refreshTimer, 0);
	glutPostRedisplay();
}


// Reshape callback Function
void viewportResize(int width, int height)
{
	Application& app = Application::getInstance();
	if (app._scene == nullptr)
		throw std::string("Active scene camera is required to change the viewport size!");

	app._applicationData.width = width;
	app._applicationData.heigth = height;
	app._scene->onViewportResize(width, height);
}


// Overlay update callback function
void displayOverlay()
{
	Application& app = Application::getInstance();
	const Application::ApplicationData& appData = app._applicationData;

	std::ostringstream oss;
	oss << appData.caption << ": " << app._frameCount << " FPS @ (" << appData.width << "x" << appData.heigth << ")";
	std::string s = oss.str();
	glutSetWindow(app._windowHandle);
	glutSetWindowTitle(s.c_str());

	app._frameCount = 0;
}


// Render callback function
void displayScene()
{
	Application& app = Application::getInstance();
	if (app._scene == nullptr)
		throw std::string("Application is missing an attached scene!");

	unsigned int currTime = glutGet(GLUT_ELAPSED_TIME);
	app._ts = (float)(currTime - app._prevTime) / 1000.0f;
	app._prevTime = currTime;

	app._frameCount++;
	app._scene->onUpdate(app._ts * app._timeScale);
	app._eventHandler.reset();

	glutSwapBuffers();
}




// Keyboard down callback function
void processKeysDown(unsigned char key, int x, int y)
{
	EventHandler& eventHandler = Application::getEventHandler();
	eventHandler.pressKey(key);
}


// Keyboard up callback function
void processKeysUp(unsigned char key, int x, int y)
{
	EventHandler& eventHandler = Application::getEventHandler();
	eventHandler.releaseKey(key);
}


// Mouse click callback function
void processMouseButtons(int button, int state, int x, int y)
{
	EventHandler& eventHandler = Application::getEventHandler();
	EventHandler::MouseStatus status = EventHandler::MouseStatus::NONE;

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
			status = EventHandler::MouseStatus::LEFT_DOWN;
		else if (button == GLUT_RIGHT_BUTTON)
			status = EventHandler::MouseStatus::RIGHT_DOWN;
	}
	else if (state == GLUT_UP)
		status = EventHandler::MouseStatus::MOUSE_UP;

	eventHandler.updateMouse(x, y);
	eventHandler.updateMouseStatus(status);
}


// Mouse motion callback function
void processMouseMotion(int x, int y)
{
	EventHandler& eventHandler = Application::getEventHandler();
	eventHandler.updateMouse(x, y);
	eventHandler.updateMouseStatus(EventHandler::MouseStatus::MOVE);
}


// Mouse wheel callback function
void mouseWheel(int wheel, int direction, int x, int y)
{
	EventHandler& eventHandler = Application::getEventHandler();
	eventHandler.updateMouse(x, y);
	eventHandler.updateMouseWeel(direction);
	eventHandler.updateMouseStatus(EventHandler::MouseStatus::SCROL);
}


#endif // !__engine_app_callback__