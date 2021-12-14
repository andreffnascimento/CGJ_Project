#include <iostream>

#include "engine/app/application.h"

#include "MicroMachines3D/scenes/MicroMachines3DScene.h"
#include "engine/scene/ecsRegistry.h"



int main(int argc, char** argv) {
	Application::ApplicationData appData = Application::ApplicationData();
	appData.caption = "CGJ Project : Micro Machines 3D";
	appData.width = 1024;
	appData.heigth = 768;
	appData.lockedFps = true;

	try 
	{
		Application &application = Application::init(appData, argc, argv);
		MicroMachines3DScene *scene = new MicroMachines3DScene();
		application.setScene(scene);
		application.run();
	} 
	catch (std::string e) 
	{
		std::cerr << "Exception: " << e << "\n";
	}
	
	return 0;
}