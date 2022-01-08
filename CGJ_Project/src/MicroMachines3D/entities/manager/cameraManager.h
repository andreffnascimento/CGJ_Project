#ifndef __mm3d_entities_manager_cameraManager__
#define __mm3d_entities_manager_cameraManager__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/camera/cameraManagerScript.h"
#include "MicroMachines3D/scripts/manager/camera/targetOrbitalCameraScript.h"




class CameraManager : public SceneEntity
{

public:
	CameraManager(Scene* scene)
		: SceneEntity(scene->createEntity("CameraManager"))
	{
		Entity camera1 = _createCamera1(scene->createEntity("Camera1"));		// camera 1 : orthographic camera with top view
		Entity camera2 = _createCamera2(scene->createEntity("Camera2"));		// camera 2 : perspective camera with top view		
		Entity camera3 = _createCamera3(scene->createEntity("Camera3"), scene);	// camera 3 : perspective camera that follows the car

		ScriptComponent& scriptComponent = addComponent<ScriptComponent>(std::make_shared<CameraManagerScript>(scene));
	}




private:
	Entity _createCamera1(Entity entity)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>(CameraComponent::CameraProjection::ORTHOGRAPHIC, ClippingPlanes({ 0.01f, 1000.0f }), 80.0f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { 0.0f, 30.0f, 0.0f });
		return entity;
	}


	Entity _createCamera2(Entity entity)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>(CameraComponent::CameraProjection::PERSPECTIVE, ClippingPlanes({ 0.01f, 1000.0f }), 53.13f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { 0.0f, 90.0f, 0.0f });
		return entity;
	}


	Entity _createCamera3(Entity entity, Scene* scene)
	{
		CameraComponent& camera = entity.addComponent<CameraComponent>(CameraComponent::CameraProjection::PERSPECTIVE, ClippingPlanes({ 0.01f, 1000.0f }), 70.0f);
		ScriptComponent& scriptComponent = entity.addComponent<ScriptComponent>(std::make_shared<TargetOrbitalCameraScript>(scene));
		return entity;
	}
};


#endif // !__mm3d_entities_manager_cameraManager__