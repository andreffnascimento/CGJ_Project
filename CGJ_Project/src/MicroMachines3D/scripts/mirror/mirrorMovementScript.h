#ifndef __mm3d_scripts_mirror_mirrorMovementScript__
#define __mm3d_scripts_mirror_mirrorMovementScript__


#include "MicroMachines3D/common/include.h"




class MirrorMovementScript : public Script
{
private:
	static constexpr float MAX_ALPHA = 180.0f;
	static constexpr float ORIGINAL_BETA = 0.0f;
	static constexpr float ORIGINAL_R = 0.0f;

private:
	CameraEntity _camera = Entity();
	const TransformComponent* _cameraTransform = nullptr;
	Entity _mirror = Entity();	
	
	float _alpha = MirrorMovementScript::MAX_ALPHA;
	float _beta = MirrorMovementScript::ORIGINAL_BETA;
	float _r = MirrorMovementScript::ORIGINAL_R;

	float _alphaAux = _alpha;
	float _betaAux = _beta;
	float _rAux = _r;


public:
	MirrorMovementScript() = delete;
	MirrorMovementScript(const MirrorMovementScript&) = default;
	MirrorMovementScript(Scene* scene) : Script(scene, "MirrorMovementScript") {}
	~MirrorMovementScript() = default;


public:
	void onCreate() override
	{
		_camera = _scene->getEntityByTag("Camera3");
		_cameraTransform = &_camera.getComponent<TransformComponent>();
		_mirror = _scene->getEntityByTag("RearViewMirror");
		_updateMirrorTransform();
	}

	void onUpdate(float ts) override
	{
		if (_scene->activeCamera() != _camera)
			return;

		_updateMirrorTransform();
	}

private:
	void _updateMirrorTransform()
	{
		
		//float orbitalCameraX = _rAux * sin(toRadians(_alphaAux)) * cos(toRadians(_betaAux));
		//float orbitalCameraZ = _rAux * cos(toRadians(_alphaAux)) * cos(toRadians(_betaAux));
		//float orbitalCameraY = _rAux * sin(toRadians(_betaAux));

		//const Coords3f& carPosition = _carTransform->translation();
		//float mirrorX = carPosition.x + orbitalCameraX;
		//float mirrorY = carPosition.y + orbitalCameraY;
		//float mirrorZ = carPosition.z + orbitalCameraZ;

		////Transform::rotate();
		//Transform::translateTo((Entity&)_mirror, { mirrorX, mirrorY, mirrorZ });
	}

};


#endif // !__mm3d_scripts_mirror_mirrorMovementScript__