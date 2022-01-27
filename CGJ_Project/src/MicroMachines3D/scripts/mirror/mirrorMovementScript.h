#ifndef __mm3d_scripts_mirror_mirrorMovementScript__
#define __mm3d_scripts_mirror_mirrorMovementScript__


#include "MicroMachines3D/common/include.h"




class MirrorMovementScript : public Script
{

private:
	static constexpr float CAM_DISTANCE = 10.0f;



private:
	Entity _car = Entity();
	Entity _mirror = Entity();
	CameraEntity _camera = CameraEntity();
	const TransformComponent* _carTransform = nullptr;
	FixedMirrorComponent* _mirrorComponent = nullptr;


public:
	MirrorMovementScript() = delete;
	MirrorMovementScript(const MirrorMovementScript&) = default;
	MirrorMovementScript(Scene* scene) : Script(scene, "MirrorMovementScript") {}
	~MirrorMovementScript() = default;


public:
	void onCreate() override
	{
		_car = _scene->getEntityByTag("Car");
		_camera = _scene->getEntityByTag("Camera3");
		_mirror = _scene->getEntityByTag("RearViewMirror");
		_carTransform = &_car.getComponent<TransformComponent>();
		_mirrorComponent = &_mirror.getComponent<FixedMirrorComponent>();

		_updateMirrorCameraPosition();
	}

	void onUpdate(float ts) override
	{
		_updateMirrorCameraPosition();
	}


private:
	void _updateMirrorCameraPosition()
	{
		const Coords3f& cameraPosition = _camera.transform().translation();
		const Coords3f& carPosition = _carTransform->translation();

		Coords3f cameraLookAt = carPosition + Coords3f{ 0.0f, 3.0f, 0.0f };
		Coords3f cameraDir = carPosition - cameraPosition;
		Coords3f mirrorCameraOffset = Coords3f{ cameraDir.x, 1.0f, cameraDir.z }.normalize() * CAM_DISTANCE;

		_mirrorComponent->setLookAt(_carTransform->translation());
		_mirrorComponent->setCameraPosition(carPosition + mirrorCameraOffset);
	}

};


#endif // !__mm3d_scripts_mirror_mirrorMovementScript__