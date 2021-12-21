#ifndef __MM3D_WORLD_CAMERAMANAGER_H__
#define __MM3D_WORLD_CAMERAMANAGER_H__


#include "MicroMachines3D/common/include.h"




class CameraManager : public SceneEntity
{

public:
	CameraManager(Scene* scene)
		: SceneEntity(scene->createEntity("CameraManager"))
	{
		// camera 1 : orthographic camera with top view
		Entity camera1 = _createCamera1(scene->createEntity("Camera1"));
		scene->setActiveCamera(Camera(camera1));

		// camera 2 : perspective camera with top view
		Entity camera2 = _createCamera2(scene->createEntity("Camera2"));
		scene->setActiveCamera(Camera(camera2));

		// camera 3 : perspective camera that follows the car
		Entity camera3 = _createCamera3(scene->createEntity("Camera3"));
		scene->setActiveCamera(Camera(camera3));
	}


private:
	Entity _createCamera1(Entity entity)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>();
		camera.setOrthographicCamera({ 0.01f, 100.0f }, { -60.0f * 1.78f, 60.0f * 1.78f, -60.0f, 60.0f });
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { 0.0f, 30.0f, 0.0f });
		return entity;
	}


	Entity _createCamera2(Entity entity)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>();
		camera.setPerspectiveCamera({ 0.01f, 1000.0f }, 53.13f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { 0.0f, 150.0f, 0.0f });
		return entity;
	}


	Entity _createCamera3(Entity entity)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>();
		camera.setPerspectiveCamera({ 0.01f, 1000.0f }, 53.13f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { 100.0f, 30.0f, 170.0f });		// TEMPORATY CODE UNTIL THE CAR IS CREATED
		return entity;
	}
};


#endif // !__MM3D_WORLD_TABLE_H__
