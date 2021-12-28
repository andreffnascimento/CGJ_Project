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
	static constexpr float MAX_ALPHA = 360.0f;
	static constexpr float ORIGINAL_BETA = 30.0f;
	static constexpr float ORIGINAL_R = 15.0f;


private:
	const EventHandler* _eventHandler = nullptr;

	Entity _car = Entity();

	CameraEntity _camera = CameraEntity();
	TargetOrbitalCameraScript::TrackingStatus _trackingStatus = TargetOrbitalCameraScript::TrackingStatus::NONE;
	int _startX = 0;
	int _startY = 0;

	float _alpha = TargetOrbitalCameraScript::MAX_ALPHA;
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
		_eventHandler = &Application::getEventHandler();
		_car = _scene->getEntityByTag("Car");
		_camera = _scene->getEntityByTag("Camera3");
		_updateCameraTransform();
		_setCameraTarget();

		_alpha = _alphaAux = _getStaticAlpha();
	}


	void onUpdate(float ts) override
	{
		const EventHandler::MouseInfo& mouseInfo = _eventHandler->getMouseInfo();
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


	void _processMouseClick(const EventHandler::MouseInfo& mouseInfo)
	{
		switch (mouseInfo.status)
		{

		case EventHandler::MouseStatus::LEFT_DOWN:
			_startX = mouseInfo.coords.x;
			_startY = mouseInfo.coords.y;
			_trackingStatus = TargetOrbitalCameraScript::TrackingStatus::MOVE;
			break;

		case EventHandler::MouseStatus::RIGHT_DOWN:
			_startX = mouseInfo.coords.x;
			_startY = mouseInfo.coords.y;
			_trackingStatus = TargetOrbitalCameraScript::TrackingStatus::ZOOM;
			break;

		case EventHandler::MouseStatus::MOUSE_UP:
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


	void _processMouseMovement(const EventHandler::MouseInfo& mouseInfo)
	{
		if (mouseInfo.status != EventHandler::MouseStatus::MOVE)
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

			_alphaAux = std::fmod(_alphaAux, TargetOrbitalCameraScript::MAX_ALPHA);
		}
		else if (_trackingStatus == TargetOrbitalCameraScript::TrackingStatus::ZOOM)
		{
			_rAux = _r + (deltaY * 0.1f);
			if (_rAux < 0.1f)
				_rAux = 0.1f;
		}
	}


	void _processMouseWheel(const EventHandler::MouseInfo& mouseInfo)
	{
		if (mouseInfo.status != EventHandler::MouseStatus::SCROL)
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

		_alphaAux += (_getStaticAlpha() - _alphaAux) * TargetOrbitalCameraScript::DECAY_SPEED * ts;
		_betaAux += (TargetOrbitalCameraScript::ORIGINAL_BETA - _betaAux) * TargetOrbitalCameraScript::DECAY_SPEED * ts;
		_rAux += (TargetOrbitalCameraScript::ORIGINAL_R - _rAux) * TargetOrbitalCameraScript::DECAY_SPEED * ts;

		_alpha = _alphaAux;
		_beta = _betaAux;
		_r = _rAux;
	}

	
	void _updateCameraTransform()
	{	
		float orbitalCameraX = _rAux * sin(toRadians(_alphaAux)) * cos(toRadians(_betaAux));
		float orbitalCameraZ = _rAux * cos(toRadians(_alphaAux)) * cos(toRadians(_betaAux));
		float orbitalCameraY = _rAux * sin(toRadians(_betaAux));

		const Coords3f& carPosition = _car.transform().translation();
		float cameraX = carPosition.x + orbitalCameraX;
		float cameraY = carPosition.y + orbitalCameraY;
		float cameraZ = carPosition.z + orbitalCameraZ;

		Transform::translateTo((Entity&)_camera, { cameraX, cameraY, cameraZ });
	}


	inline float _getStaticAlpha() { return _car.transform().rotation().y + 180.0f; }

};


#endif // !__mm3d_scripts_camera_targetOrbitalCameraScript__