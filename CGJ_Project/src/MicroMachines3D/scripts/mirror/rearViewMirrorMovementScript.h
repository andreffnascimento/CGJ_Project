#ifndef __mm3d_scripts_mirror_rearViewMirrorMovementScript__
#define __mm3d_scripts_mirror_rearViewMirrorMovementScript__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/manager/game/raceManagerScript.h"




class RearViewMirrorMovementScript : public Script
{

private:
	static constexpr float CAM_Y_OFFSET = 3.0f;




private:
	const EventHandler* _eventHandler = nullptr;

	CameraEntity _camera = CameraEntity();

	FlatMirrorComponent* _mirrorComponent = nullptr;
	const TransformComponent* _carTransform = nullptr;

	const RaceManagerScript* _raceManagerScript = nullptr;

	bool _mirrorEnabled = true;




public:
	RearViewMirrorMovementScript() = delete;
	RearViewMirrorMovementScript(const RearViewMirrorMovementScript&) = default;
	RearViewMirrorMovementScript(Scene* scene) : Script(scene, "MirrorMovementScript") {}
	~RearViewMirrorMovementScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_camera = _scene->getEntityByTag("Camera3");
		_carTransform = &_scene->getEntityByTag("Car").getComponent<TransformComponent>();
		_mirrorComponent = &_scene->getEntityByTag("RearViewMirror").getComponent<FlatMirrorComponent>();
		_raceManagerScript = dynamic_cast<RaceManagerScript*>(_scene->getEntityByTag("GameManager").getComponent<ScriptComponent>().getScriptByTag("RaceManagerScript"));

		_updateMirrorCameraPosition();
	}

	void onUpdate(float ts) override
	{
		if (!_raceManagerScript->playing())
			return;

		if (_eventHandler->keyState('M').pressed() || _eventHandler->keyState('m').pressed())
			_mirrorEnabled = !_mirrorEnabled;

		_mirrorComponent->setEnabled(_mirrorEnabled && _scene->activeCamera() == _camera);
		if (_mirrorComponent->enabled())
			_updateMirrorCameraPosition();
	}




private:
	void _updateMirrorCameraPosition()
	{
		float carRotation = _carTransform->rotation().toEulerAngles().y;
		Coords3f mirrorLookAt = Quaternion(Coords3f({ 0.0f, 1.0f, 0.0f }), carRotation - 90.0f).calculatePointRotation(Coords3f({1.0f, 0.0f, 0.0f}));

		Coords3f cameraPosition = _carTransform->translation() + mirrorLookAt * 5.0f;
		cameraPosition.y += RearViewMirrorMovementScript::CAM_Y_OFFSET;

		_mirrorComponent->cameraComponent().setTargetCoords(_carTransform->translation());
		Transform::translateTo(_mirrorComponent->camera(), cameraPosition);
	}

};


#endif // !__mm3d_scripts_mirror_rearViewMirrorMovementScript__