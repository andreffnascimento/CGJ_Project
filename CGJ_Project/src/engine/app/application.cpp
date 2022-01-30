#include "application.h"

#include "engine/app/callback.h"

#include <GL/freeglut.h>	// GLUT is the toolkit to interface with the OS




Application* Application::s_application = nullptr;




Application::~Application()
{
	glutLeaveMainLoop();
}




Application& Application::init(const ApplicationData& applicationData, int argc, char** argv)
{
	// checks if the application was previously instantiated
	if (Application::s_application != nullptr)
		return Application::getInstance();

	// instantiates the application
	Application::s_application = new Application();
	Application& app = *Application::s_application;

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
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_STENCIL);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(appData.width, appData.heigth);
	app._windowHandle = glutCreateWindow(appData.caption.c_str());

	// window and display callback registration
	glutCloseFunc(terminateApp);
	glutTimerFunc(0, overlayTimer, 0);
	glutReshapeFunc(viewportResize);
	glutDisplayFunc(displayScene);

	if (appData.lockedFps)
		glutTimerFunc(0, refreshTimer, 0);
	else
		glutIdleFunc(displayScene);

	// mouse and keyboard callback registration
	glutKeyboardFunc(processKeysDown);
	glutKeyboardUpFunc(processKeysUp);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);

	// stop repeating keyboard events
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	app._originalWindowSize = WindowCoords({ appData.width, appData.heigth });

	app._renderer.init();				// initializes the renderer
	return app;
}


void Application::terminate()
{
	if (Application::s_application != nullptr)
	{
		delete Application::s_application;
		Application::s_application = nullptr;
	}
}




bool Application::isRunning()
{
	return Application::s_application != nullptr && Application::s_application->_running;
}

Application& Application::getInstance()
{
	return Application::s_application != nullptr
		? *Application::s_application
		: throw std::string("Application hasn't been initialized yet!");
}

EventHandler& Application::getEventHandler()
{
	Application& app = Application::getInstance();
	return app._eventHandler;
}

PhysicsEngine& Application::getPhysicsEngine()
{
	Application& app = Application::getInstance();
	return app._physicsEngine;
}

Renderer& Application::getRenderer()
{
	Application& app = Application::getInstance();
	return app._renderer;
}




void Application::setScene(Scene* scene)
{
	_scene = scene;
	_scene->setupScene();
	_scene->onCreate();

	_running = true;
}


void Application::run()
{
	if (Application::isRunning())
		glutMainLoop();
}




void Application::setTimeScale(float timeScale)
{
	if (timeScale < 0.0f)
		throw std::string("The time scale must be a non negative float value!");

	_timeScale = timeScale;
}
