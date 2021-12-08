#include "MicroMachines3DScene.h"

#include "MicroMachines3D/gameObjects/camera/orthographicCamera.h"
#include "MicroMachines3D/gameObjects/camera/perspectiveCamera.h"



MicroMachines3DScene::MicroMachines3DScene()
	: Scene()
{
	// empty
}




void MicroMachines3DScene::_init()
{
	_initCameras();
}



void MicroMachines3DScene::_initCameras()
{
	// Add scene cameras
	OrthographicCamera* camera1 = new OrthographicCamera(-85.0f, 85.0f, -50.0f, 50.0f, 0.01f, 1000.0f);
	camera1->setPosition(0.0f, 50.0f, 0.0f);
	camera1->setTarget(0.0f, 0.0f, 0.0f);
	addGameObject(camera1);
	setActiveCamera(camera1);

	PerspectiveCamera* camera2 = new PerspectiveCamera(70.0f, 0.01f, 1000.0f);
	camera2->setPosition(0.0f, 50.0f, 0.0f);
	camera2->setTarget(0.0f, 0.0f, 0.0f);
	addGameObject(camera2);

	//PerspectiveCamera* camera3 = new PerspectiveCamera(70.0f, 0.01f, 1000.0f);
	//camera3->setPosition(0.0f, 50.0f, 0.0f);
	//camera3->setTarget(0.0f, 0.0f, 0.0f);
	//addGameObject(camera2);
}