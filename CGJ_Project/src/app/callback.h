#ifndef APP_CALLBACK_H
#define APP_CALLBACK_H


#include <iostream>
#include <sstream>

#include "application.h"
#include "test.h"


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

	float res[4];
	float lightPos[4] = { 4.0f, 6.0f, 2.0f, 1.0f };
	multMatrixPoint(VIEW, lightPos, res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(getUniformLocation(UniformType::L_POS), 1, res);

	app.updateGame();
	renderTable();

	glDisable(GL_DEPTH_TEST);

	//the glyph contains background colors and non-transparent for the actual character pixels. So we use the blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	//viewer at origin looking down at  negative z direction
	pushMatrix(MODEL);
	loadIdentity(MODEL);
	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	pushMatrix(VIEW);
	loadIdentity(VIEW);
	ortho(m_viewport[0], m_viewport[0] + m_viewport[2] - 1, m_viewport[1], m_viewport[1] + m_viewport[3] - 1, -1, 1);
	RenderText(getTextShader(), "This is a sample text", 25.0f, 25.0f, 1.0f, 0.5f, 0.8f, 0.2f);
	RenderText(getTextShader(), "CGJ Light and Text Rendering Demo", 440.0f, 570.0f, 0.5f, 0.3, 0.7f, 0.9f);
	popMatrix(PROJECTION);
	popMatrix(VIEW);
	popMatrix(MODEL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

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
}


// Mouse wheel callback function
void mouseWheel(int wheel, int direction, int x, int y) 
{
	InputHandler &input = Application::getInputHandler();
	input.updateMouse(x, y);
	input.updateMouseWeel(direction);
}


#endif // !APP_CALLBACK_H