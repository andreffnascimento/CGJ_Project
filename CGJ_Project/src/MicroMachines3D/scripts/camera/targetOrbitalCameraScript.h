#ifndef __mm3d_scripts_camera_targetOrbitalCameraScript__
#define __mm3d_scripts_camera_targetOrbitalCameraScript__


#include "MicroMachines3D/common/include.h"




class TargetOrbitalCameraScript : public Script
{

private:
	enum class TrackingStatus
	{
		NONE,
		MOVE,
		ZOOM
	};




private:
	static constexpr float DECAY_SPEED = 1.0f;
	static constexpr float ORIGINAL_ALPHA = 90.0f;
	static constexpr float ORIGINAL_BETA = 30.0f;
	static constexpr float ORIGINAL_R = 15.0f;


private:
	const InputHandler* _inputHandler = nullptr;

	Entity _car = Entity();

	Camera _camera = Camera();
	TargetOrbitalCameraScript::TrackingStatus _trackingStatus = TargetOrbitalCameraScript::TrackingStatus::NONE;
	int _startX = 0;
	int _startY = 0;

	float _alpha = TargetOrbitalCameraScript::ORIGINAL_ALPHA;
	float _beta = TargetOrbitalCameraScript::ORIGINAL_BETA;
	float _r = TargetOrbitalCameraScript::ORIGINAL_R;

	float _alphaAux = _alpha;
	float _betaAux = _beta;
	float _rAux = _r;




public:
	TargetOrbitalCameraScript() = delete;
	TargetOrbitalCameraScript(const TargetOrbitalCameraScript&) = default;
	TargetOrbitalCameraScript(Scene* scene) : Script(scene) {}
	~TargetOrbitalCameraScript() = default;


public:
	void onCreate() override
	{
		_inputHandler = &Application::getInputHandler();
		_car = _scene->getEntityByTag("Car");
		_camera = _scene->getEntityByTag("Camera3");
		_updateCameraTransform();
		_setCameraTarget();
	}


	void onUpdate(float ts) override
	{
		const InputHandler::MouseInfo& mouseInfo = _inputHandler->getMouseInfo();
		_processMouseClick(mouseInfo);
		_processMouseMovement(mouseInfo);
		_processMouseWheel(mouseInfo);
		_decayOrbitalValuesToOriginal(ts);
		_updateCameraTransform();
		_setCameraTarget();
	}


private:
	void _setCameraTarget() const
	{
		CameraComponent& cameraComponent = _camera.getComponent<CameraComponent>();
		cameraComponent.setTargetCoords(_car.transform().translation());
	}


	void _processMouseClick(const InputHandler::MouseInfo& mouseInfo)
	{
		switch (mouseInfo.status)
		{

		case InputHandler::MouseStatus::LEFT_DOWN:
			_startX = mouseInfo.coords.x;
			_startY = mouseInfo.coords.y;
			_trackingStatus = TargetOrbitalCameraScript::TrackingStatus::MOVE;
			break;

		case InputHandler::MouseStatus::RIGHT_DOWN:
			_startX = mouseInfo.coords.x;
			_startY = mouseInfo.coords.y;
			_trackingStatus = TargetOrbitalCameraScript::TrackingStatus::ZOOM;
			break;

		case InputHandler::MouseStatus::MOUSE_UP:
			if (_trackingStatus == TargetOrbitalCameraScript::TrackingStatus::MOVE)
			{
				_alpha += -(int)mouseInfo.coords.x + _startX;
				_beta +=   (int)mouseInfo.coords.y - _startY;
			}
			else if (_trackingStatus == TargetOrbitalCameraScript::TrackingStatus::ZOOM)
			{
				_r += (mouseInfo.coords.y - _startY) * 0.1f;
				if (_r < 0.1f)
					_r = 0.01f;
			}

			_trackingStatus = TargetOrbitalCameraScript::TrackingStatus::NONE;
			break;
		}
	}


	void _processMouseMovement(const InputHandler::MouseInfo& mouseInfo)
	{
		if (mouseInfo.status != InputHandler::MouseStatus::MOVE)
			return;

		_alphaAux = _alpha;
		_betaAux = _beta;
		_rAux = _r;

		int deltaX = -(int)mouseInfo.coords.x + _startX;
		int deltaY =  (int)mouseInfo.coords.y - _startY;

		if (_trackingStatus == TargetOrbitalCameraScript::TrackingStatus::MOVE)
		{
			_alphaAux = _alpha + deltaX;
			_betaAux = _beta + deltaY;

			if (_betaAux > 85.0f)
				_betaAux = 85.0f;
			else if (_betaAux < -85.0f)
				_betaAux = -85.0f;
		}
		else if (_trackingStatus == TargetOrbitalCameraScript::TrackingStatus::ZOOM)
		{
			_rAux = _r + (deltaY * 0.1f);
			if (_rAux < 0.1f)
				_rAux = 0.1f;
		}
	}


	void _processMouseWheel(const InputHandler::MouseInfo& mouseInfo)
	{
		if (mouseInfo.status != InputHandler::MouseStatus::SCROL)
			return;

		_rAux -= mouseInfo.wheelDirection * 1.0f;
		if (_rAux < 0.1f)
			_rAux = 0.1f;

		_r = _rAux;
	}


	void _decayOrbitalValuesToOriginal(float ts)
	{
		if (_trackingStatus != TargetOrbitalCameraScript::TrackingStatus::NONE && true)		// FIXME: check if car is moving
			return;

		_alphaAux += (TargetOrbitalCameraScript::ORIGINAL_ALPHA - _alphaAux) * TargetOrbitalCameraScript::DECAY_SPEED * ts;
		_betaAux += (TargetOrbitalCameraScript::ORIGINAL_BETA - _betaAux) * TargetOrbitalCameraScript::DECAY_SPEED * ts;
		_rAux += (TargetOrbitalCameraScript::ORIGINAL_R - _rAux) * TargetOrbitalCameraScript::DECAY_SPEED * ts;

		_alpha = _alphaAux;
		_beta = _betaAux;
		_r = _rAux;
	}

	
	void _updateCameraTransform()
	{	
		float orbitalCameraX = _rAux * sin(_alphaAux * (float)Transform::PI / 180.0f) * cos(_betaAux * (float)Transform::PI / 180.0f);
		float orbitalCameraZ = _rAux * cos(_alphaAux * (float)Transform::PI / 180.0f) * cos(_betaAux * (float)Transform::PI / 180.0f);
		float orbitalCameraY = _rAux * sin(_betaAux *  (float)Transform::PI / 180.0f);

		const Coords3f& carPosition = _car.transform().translation();
		float cameraX = carPosition.x + orbitalCameraX;
		float cameraY = carPosition.y + orbitalCameraY;
		float cameraZ = carPosition.z + orbitalCameraZ;

		Transform::translateTo((Entity&)_camera, { cameraX, cameraY, cameraZ });
	}

};


#endif // !__mm3d_scripts_camera_targetOrbitalCameraScript__