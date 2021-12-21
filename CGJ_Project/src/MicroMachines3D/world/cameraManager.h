#ifndef __mm3d_world_cameraManager__
#define __mm3d_world_cameraManager__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/camera/cameraManagerScript.h"
#include "MicroMachines3D/scripts/camera/targetOrbitalCameraScript.h"




class CameraManager : public SceneEntity
{

public:
	CameraManager(Scene* scene)
		: SceneEntity(scene->createEntity("CameraManager"))
	{
		Entity camera1 = _createCamera1(scene->createEntity("Camera1"));		// camera 1 : orthographic camera with top view
		Entity camera2 = _createCamera2(scene->createEntity("Camera2"));		// camera 2 : perspective camera with top view		
		Entity camera3 = _createCamera3(scene->createEntity("Camera3"), scene);	// camera 3 : perspective camera that follows the car

		std::shared_ptr<Script> script = std::make_shared<CameraManagerScript>(scene);
		ScriptComponent& scriptComponent = addComponent<ScriptComponent>(script);
	}


private:
	Entity _createCamera1(Entity entity)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>();
		camera.setOrthographicCamera({ 0.01f, 1000.0f }, 120.0f);
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


	Entity _createCamera3(Entity entity, Scene* scene)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>();
		camera.setPerspectiveCamera({ 0.01f, 1000.0f }, 53.13f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		
		std::shared_ptr<Script> script = std::make_shared<TargetOrbitalCameraScript>(scene);
		ScriptComponent& scriptComponent = entity.addComponent<ScriptComponent>(script);

		return entity;
	}
};


#endif // !__mm3d_world_cameraManager__