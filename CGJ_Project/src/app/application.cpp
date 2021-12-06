#include "application.h"
#include "callback.h"



const char *Application::CAPTION = "CGJ Project: Micro Machines 3D";	// window caption
const char* Application::FONT_NAME = "fonts/arial.ttf";				// font name





Application::Application()
{
	_inputHandler = InputHandler();
}





Application& Application::getInstance()
{
	static Application app;
	return app;
}



InputHandler& Application::getInputHandler()
{
	Application app = Application::getInstance();
	return app._inputHandler;
}



void Application::init(int argc, char** argv, bool lockedFps)
{
	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Application::WIDTH, Application::HEIGHT);
	_windowHandle = glutCreateWindow(CAPTION);

	// callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutTimerFunc(0, timer, 0);

	if (lockedFps)
		glutTimerFunc(0, refresh, 0);   //use it to to get 60 FPS whatever
	else
		glutIdleFunc(renderScene);		// Use it for maximum performance

	// mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);

	// return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// print engine information
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	// initialize the game
	_game = Game();

	// initialization of DevIL
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		printf("wrong DevIL version \n");
		exit(0);
	}
	ilInit();

	// initialization of freetype library with font_name file
	freeType_init(Application::FONT_NAME);

	// initialize the shaders
	if (!setupShaders())
		throw std::string("Unable to initialize the shaders");

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void Application::run()
{
	//  GLUT main loop
	glutMainLoop();
}