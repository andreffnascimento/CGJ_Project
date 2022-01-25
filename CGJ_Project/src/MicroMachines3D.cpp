#include <iostream>

#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scenes/MicroMachines3DScene.h"



int main(int argc, char** argv) {
	Application::ApplicationData appData = Application::ApplicationData();
	appData.caption = "CGJ Project : Micro Machines 3D";
	appData.width = 1280;
	appData.heigth = 720;
	appData.lockedFps = false;

	try 
	{
		Application &application = Application::init(appData, argc, argv);
		MicroMachines3DScene scene = MicroMachines3DScene();
		application.setScene(&scene);
		application.run();
	} 
	catch (std::string e)
	{
		std::cerr << "Exception: " << e << "\n";
	}
	
	return 0;
}