#include "MicroMachines3DScene.h"

#include "MicroMachines3D/gameObjects/camera/orthographicCamera.h"
#include "MicroMachines3D/gameObjects/camera/perspectiveCamera.h"
#include "MicroMachines3D/gameObjects/world/table.h"



MicroMachines3DScene::MicroMachines3DScene()
	: Scene()
{
	_init();
}



#include <iostream>
void MicroMachines3DScene::_init()
{
	_initCameras();
	_initTable();
}



void MicroMachines3DScene::_initCameras()
{
	OrthographicCamera* camera1 = new OrthographicCamera(-97.7, 97.7f, -55.0f, 55.0f, 0.1f, 1000.0f);
	camera1->setPosition(0.0f, 70.0f, 0.0f);
	camera1->setTarget(0.0f, 0.0f, 0.0f);
	addEntity(camera1);
	setActiveCamera(camera1);

	PerspectiveCamera* camera2 = new PerspectiveCamera(53.13f, 0.01f, 1000.0f);
	camera2->setPosition(150.0f, 30.0f, 150.0f);	// FUTURE: y = 150
	camera2->setTarget(0.0f, 0.0f, 0.0f);
	addEntity(camera2);
	setActiveCamera(camera2);

	//PerspectiveCamera* camera3 = new PerspectiveCamera(70.0f, 0.01f, 1000.0f);
	//camera3->setPosition(0.0f, 50.0f, 0.0f);
	//camera3->setTarget(0.0f, 0.0f, 0.0f);
	//addGameObject(camera2);
}


void MicroMachines3DScene::_initTable()
{
	Table* table = new Table();
	addEntity(table);
}