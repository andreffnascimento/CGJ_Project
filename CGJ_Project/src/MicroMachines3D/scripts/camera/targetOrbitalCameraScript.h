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
	const InputHandler* _inputHandler = nullptr;

	Entity _car = Entity();

	Camera _camera = Camera();
	TrackingStatus _trackingStatus = TrackingStatus::NONE;
	int _startX = 0;
	int _startY = 0;

	float _alpha = 180.0f;
	float _beta = 50.0f;
	float _r = 10.0f;




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
		_updateCameraTransform(_alpha, _beta, _r);
	}


	void onUpdate(float ts) override
	{
		const InputHandler::MouseInfo& mouseInfo = _inputHandler->getMouseInfo();
		_processMouseClick(mouseInfo);
		_processMouseMovement(mouseInfo);
		_processMouseWheel(mouseInfo);
		_setCameraTarget();
	}


private:
	void _processMouseClick(const InputHandler::MouseInfo& mouseInfo)
	{
		switch (mouseInfo.status)
		{

		case InputHandler::MouseStatus::LEFT_DOWN:
			_startX = mouseInfo.coords.x;
			_startY = mouseInfo.coords.y;
			_trackingStatus = TrackingStatus::MOVE;
			break;

		case InputHandler::MouseStatus::RIGHT_DOWN:
			_startX = mouseInfo.coords.x;
			_startY = mouseInfo.coords.y;
			_trackingStatus = TrackingStatus::ZOOM;
			break;

		case InputHandler::MouseStatus::MOUSE_UP:
			if (_trackingStatus == TrackingStatus::MOVE)
			{
				_alpha += -(int)mouseInfo.coords.x + _startX;
				_beta +=   (int)mouseInfo.coords.y - _startY;
			}
			else if (_trackingStatus == TrackingStatus::ZOOM)
			{
				_r += (mouseInfo.coords.y - _startY) * 0.1f;
				if (_r < 0.1f)
					_r = 0.01f;
			}

			_trackingStatus = TrackingStatus::NONE;
			break;
		}
	}


	void _processMouseMovement(const InputHandler::MouseInfo& mouseInfo)
	{
		if (mouseInfo.status != InputHandler::MouseStatus::MOVE)
			return;

		float alphaAux = _alpha;
		float betaAux = _beta;
		float rAux = _r;

		int deltaX = -(int)mouseInfo.coords.x + _startX;
		int deltaY =  (int)mouseInfo.coords.y - _startY;

		if (_trackingStatus == TrackingStatus::MOVE)
		{
			alphaAux = _alpha + deltaX;
			betaAux = _beta + deltaY;

			if (betaAux > 85.0f)
				betaAux = 85.0f;
			else if (betaAux < -85.0f)
				betaAux = -85.0f;
		}
		else if (_trackingStatus == TrackingStatus::ZOOM)
		{
			rAux = _r + (deltaY * 0.1f);
			if (rAux < 0.1f)
				rAux = 0.1f;
		}

		_updateCameraTransform(alphaAux, betaAux, rAux);
	}


	void _processMouseWheel(const InputHandler::MouseInfo& mouseInfo)
	{
		if (mouseInfo.status != InputHandler::MouseStatus::SCROL)
			return;

		_r -= mouseInfo.wheelDirection * 1.0f;
		if (_r < 0.1f)
			_r = 0.1f;

		_updateCameraTransform(_alpha, _beta, _r);
	}


	void _updateCameraTransform(float alpha, float beta, float r)
	{
		float cameraX = r * sin(alpha * (float)Transform::PI / 180.0f) * cos(beta * (float)Transform::PI / 180.0f);
		float cameraZ = r * cos(alpha * (float)Transform::PI / 180.0f) * cos(beta * (float)Transform::PI / 180.0f);
		float cameraY = r * sin(beta *  (float)Transform::PI / 180.0f);
		Transform::translateTo((Entity&)_camera, { cameraX, cameraY, cameraZ });
	}


	void _setCameraTarget() const
	{
		CameraComponent& cameraComponent = _camera.getComponent<CameraComponent>();
		cameraComponent.setTargetCoords(_car.transform().translation());
	}

};


#endif // !__mm3d_scripts_camera_targetOrbitalCameraScript__