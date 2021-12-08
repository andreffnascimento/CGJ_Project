#include "application.h"

#include <iostream>

#include "engine/app/callback.h"

#include <GL/freeglut.h>	// GLUT is the toolkit to interface with the OS




Application* Application::s_application = nullptr;




Application::Application()
	: _applicationData(Application::ApplicationData()), _running(false), _windowHandle(0), _frameCount(0), 
	  _inputHandler(InputHandler()), _renderer(nullptr), _scene(nullptr) 
{
	// empty
}

Application::~Application()
{
	_running = false;
	glutLeaveMainLoop();

	delete _renderer;
	delete _scene;
}




Application& Application::init(const ApplicationData &applicationData, int argc, char** argv)
{
	// checks if the application was previously instantiated
	if (Application::s_application != nullptr)
		return Application::getInstance();

	// instantiates the application
	Application::s_application = new Application();
	Application &app = *Application::s_application;

	// instantiates the application data
	Application::ApplicationData &appData = app._applicationData;
	appData.caption = applicationData.caption;
	appData.width = applicationData.width;
	appData.heigth = applicationData.heigth;
	appData.lockedFps = applicationData.lockedFps;
	
	// store the data of the application 
	app._applicationData = applicationData;

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(appData.width, appData.heigth);
	app._windowHandle = glutCreateWindow(appData.caption.c_str());

	// callback Registration
	glutCloseFunc(terminateApp);
	glutDisplayFunc(displayScene);
	glutReshapeFunc(changeViewportSize);
	glutTimerFunc(0, timer, 0);

	if (appData.lockedFps)
		glutTimerFunc(0, refresh, 0);   //use it to to get 60 FPS whatever
	else
		glutIdleFunc(displayScene);		// Use it for maximum performance

	// mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);

	// return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	app._renderer = new Renderer();		// initializes the renderer
	app._running = true;				// starts the app
	return app;
}


void Application::terminate()
{
	if (Application::s_application != nullptr)
		delete Application::s_application;
}




Application& Application::getInstance()
{
	return Application::s_application != nullptr
		? *Application::s_application
		: throw std::string("Application hasn't been initialized yet!");
}

InputHandler& Application::getInputHandler()
{
	Application &app = Application::getInstance();
	return app._inputHandler;
}




void Application::setScene(Scene* scene)
{
	_scene = scene;
}


void Application::run()
{
	if (_running)
		glutMainLoop();
}