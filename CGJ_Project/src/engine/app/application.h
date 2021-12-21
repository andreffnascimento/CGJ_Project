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
	ApplicationData _applicationData = Application::ApplicationData();

	bool _running = false;
	int _windowHandle = 0;
	unsigned int _frameCount = 0;

	InputHandler _inputHandler = InputHandler();
	Renderer _renderer = Renderer();
	Scene* _scene;




private:
	Application() = default;		// application is a singleton


public:
	~Application();
	

public:
	static Application& init(const ApplicationData& applicationData, int argc, char** argv);
	static void terminate();


public:
	static Application& getInstance();
	static InputHandler& getInputHandler();
	static Renderer& getRenderer();


public:
	inline WindowCoords getWindowSize() const { return { _applicationData.width, _applicationData.heigth }; }


public:
	void setScene(Scene* scene);
	void run();




public:
	friend void timer(int value);
	friend void refresh(int value);
	friend void viewportResize(int width, int height);
	friend void displayScene();

};


#endif // !__ENGINE_APP_APPLICATION_H__