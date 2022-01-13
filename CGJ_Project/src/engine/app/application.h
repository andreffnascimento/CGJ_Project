#ifndef __engine_app_application__
#define __engine_app_application__


#include <string>

#include "engine/app/eventHandler.h"

#include "engine/physics/physicsEngine.h"
#include "engine/renderer/renderer.h"
#include "engine/scene/scene.h"
#include "engine/utils/coords.h"




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

	unsigned int _prevTime = 0;
	float _ts = 0.0;
	float _timeScale = 1.0f;

	EventHandler _eventHandler = EventHandler();
	PhysicsEngine _physicsEngine = PhysicsEngine();
	Renderer _renderer = Renderer();
	Scene* _scene = nullptr;




private:
	Application() = default;		// application is a singleton
	Application(const Application& application) = default;


public:
	~Application();
	

public:
	static Application& init(const ApplicationData& applicationData, int argc, char** argv);
	static void terminate();


public:
	static bool isRunning();
	static Application& getInstance();
	static EventHandler& getEventHandler();
	static PhysicsEngine& getPhysicsEngine();
	static Renderer& getRenderer();


public:
	inline WindowCoords getWindowSize() const { return { _applicationData.width, _applicationData.heigth }; }


public:
	void setScene(Scene* scene);
	void run();


public:
	void setTimeScale(float timeScale);




public:
	friend void terminateApp();
	friend void overlayTimer(int value);
	friend void refreshTimer(int value);
	friend void viewportResize(int width, int height);
	friend void displayOverlay();
	friend void displayScene();

};


#endif // !__engine_app_application__