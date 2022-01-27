#ifndef __mm3d_scripts_mirror_mirrorMovementScript__
#define __mm3d_scripts_mirror_mirrorMovementScript__


#include "MicroMachines3D/common/include.h"




class MirrorMovementScript : public Script
{
private:
	Entity _car = Entity();
	CameraEntity _camera = CameraEntity();
	const TransformComponent* _carTransform = nullptr;
	Entity _mirror = Entity();


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
		_carTransform = &_car.getComponent<TransformComponent>();
		_mirror = _scene->getEntityByTag("RearViewMirror");

		_updateMirrorTransform();
	}

	void onUpdate(float ts) override
	{
		_updateMirrorTransform();
	}

private:
	void _updateMirrorTransform()
	{
		const Coords3f& cameraPosition = _camera.transform().translation();
		const Coords3f& carPosition = _carTransform->translation();

		Coords3f cameraDir = carPosition - cameraPosition;


		float mirrorX = carPosition.x + cameraDir.x;
		float mirrorY = carPosition.y - cameraDir.y;
		float mirrorZ = carPosition.z + cameraDir.y;

		//Transform::rotateTo((Entity&)_mirror, _camera.transform().rotation());
		Transform::translateTo((Entity&)_mirror, { mirrorX, mirrorY, mirrorZ });
	}

};


#endif // !__mm3d_scripts_mirror_mirrorMovementScript__