#include "app/application.h"



int main(int argc, char** argv) {

	Application &application = Application::getInstance();
	
	try 
	{
		application.init(argc, argv, true);
		application.run();
	} 
	catch (std::string e) 
	{
		std::cerr << "Exception: " << e << "\n";
	}

	return 0;
}