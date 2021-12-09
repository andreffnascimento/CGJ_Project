#ifndef __MM3D_GAMEOBJECTS_MANAGER_CAMERAMANAGER_H__
#define __MM3D_GAMEOBJECTS_MANAGER_CAMERAMANAGER_H__


#include "engine/scene/entity.h"

#include "MicroMachines3D/gameObjects/camera/orthographicCamera.h"
#include "MicroMachines3D/gameObjects/camera/perspectiveCamera.h"




class CameraManager : public Entity
{

public:
	CameraManager(Scene &scene)
	{
		OrthographicCamera* camera1 = new OrthographicCamera(-97.7f, 97.7f, -55.0f, 55.0f, 0.1f, 1000.0f);
		camera1->setPosition(0.0f, 70.0f, 0.0f);
		camera1->setTarget(0.0f, 0.0f, 0.0f);
		addChild(camera1);
		scene.setActiveCamera(camera1);

		PerspectiveCamera* camera2 = new PerspectiveCamera(53.13f, 0.01f, 1000.0f);
		camera2->setPosition(100.0f, 10.0f, 170.0f);	// FUTURE: y = 150
		camera2->setTarget(0.0f, 0.0f, 0.0f);
		addChild(camera2);
		scene.setActiveCamera(camera2);

		//PerspectiveCamera* camera3 = new PerspectiveCamera(70.0f, 0.01f, 1000.0f);
		//camera3->setPosition(0.0f, 50.0f, 0.0f);
		//camera3->setTarget(0.0f, 0.0f, 0.0f);
		//addGameObject(camera2);*/

		// TODO: add camera script
	}

};


#endif // !__MM3D_GAMEOBJECTS_MANAGER_CAMERAMANAGER_H__
