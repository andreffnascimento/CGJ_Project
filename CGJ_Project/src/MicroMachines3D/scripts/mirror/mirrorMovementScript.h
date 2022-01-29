#ifndef __mm3d_scripts_mirror_mirrorMovementScript__
#define __mm3d_scripts_mirror_mirrorMovementScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"




class MirrorMovementScript : public Script
{

private:
	static constexpr float CAM_DISTANCE = 10.0f;




private:
	const EventHandler* _eventHandler = nullptr;

	CameraEntity _camera = CameraEntity();

	FixedMirrorComponent* _mirrorComponent = nullptr;
	const TransformComponent* _carTransform = nullptr;

	const RaceManagerScript* _raceManagerScript = nullptr;

	bool _mirrorEnabled = true;




public:
	MirrorMovementScript() = delete;
	MirrorMovementScript(const MirrorMovementScript&) = default;
	MirrorMovementScript(Scene* scene) : Script(scene, "MirrorMovementScript") {}
	~MirrorMovementScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_camera = _scene->getEntityByTag("Camera3");
		_carTransform = &_scene->getEntityByTag("Car").getComponent<TransformComponent>();
		_mirrorComponent = &_scene->getEntityByTag("RearViewMirror").getComponent<FixedMirrorComponent>();
		_raceManagerScript = dynamic_cast<RaceManagerScript*>(_scene->getEntityByTag("GameManager").getComponent<ScriptComponent>().getScriptByTag("RaceManagerScript"));

		_updateMirrorCameraPosition();
	}

	void onUpdate(float ts) override
	{
		if (_raceManagerScript->paused())
			return;

		if (_scene->activeCamera() != _camera)
			_mirrorComponent->setEnabled(_mirrorEnabled = false);

		if (_eventHandler->keyState('M').pressed() || _eventHandler->keyState('m').pressed())
			_mirrorComponent->setEnabled(_mirrorEnabled = !_mirrorEnabled);
		
		if (_mirrorEnabled)
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