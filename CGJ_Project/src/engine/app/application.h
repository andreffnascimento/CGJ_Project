#ifndef __ENGINE_APP_APPLICATION_H__
#define __ENGINE_APP_APPLICATION_H__


#include <string>

#include "engine/app/inputHandler.h"
#include "engine/renderer/renderer.h"
#include "engine/scene/scene.h"




class Application 
{

public:
	struct ApplicationData 
	{
		std::string caption = "";
		unsigned int width = 0;
		unsigned int heigth = 0;
		bool lockedFps = true;
	};




private:
	static Application* s_application;


private:
	ApplicationData _applicationData;

	bool _running;
	int _windowHandle;
	unsigned int _frameCount;

	InputHandler _inputHandler;
	Renderer *_renderer;
	Scene *_scene;




private:
	Application();		// application is a singleton


public:
	~Application();
	

public:
	static Application& init(const ApplicationData &applicationData, int argc, char** argv);
	static void terminate();

public:
	static Application& getInstance();
	static InputHandler& getInputHandler();


public:
	void setScene(Scene* scene);
	void run();


public:
	friend void timer(int value);
	friend void refresh(int value);
	friend void changeViewportSize(int width, int height);
	friend void displayScene();

};


#endif // !__ENGINE_APP_APPLICATION_H__