#include "worldCameraScript.h"



WorldCameraScript::WorldCameraScript(Camera &camera, InputHandler& input)
	: _camera(camera), _input(input), _startX(0), _startY(0), _trackingStatus(TrackingStatus::NONE)
{
	// empty
}




void WorldCameraScript::run()
{
	const InputHandler::MouseInfo& mouseInfo = _input.getMouseInfo();
	_processMouseClick(mouseInfo);
	_processMouseMovement(mouseInfo);
	_processMouseWheel(mouseInfo);
}


void WorldCameraScript::_processMouseClick(const InputHandler::MouseInfo& mouseInfo)
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
			_camera._alpha -= (mouseInfo.coords.x - _startX);
			_camera._beta  += (mouseInfo.coords.y - _startY);
		}
		else if (_trackingStatus == TrackingStatus::ZOOM)
		{
			_camera._r += (mouseInfo.coords.y - _startY) * 0.01f;
			if (_camera._r < 0.1f)
				_camera._r = 0.01f;
		}
		_trackingStatus = TrackingStatus::NONE;
		break;

	}
}


void WorldCameraScript::_processMouseMovement(const InputHandler::MouseInfo& mouseInfo)
{
	if (mouseInfo.status != InputHandler::MouseStatus::MOVE)
		return;

	float alphaAux = _camera._alpha;
	float betaAux = _camera._beta;
	float rAux = _camera._r;

	int deltaX = -mouseInfo.coords.x + _startX;
	int deltaY = mouseInfo.coords.y - _startY;

	if (_trackingStatus == TrackingStatus::MOVE)
	{
		alphaAux = _camera._alpha + deltaX;
		betaAux = _camera._beta + deltaY;
		rAux = _camera._r;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
	}
	else if (_trackingStatus == TrackingStatus::ZOOM)
	{
		alphaAux = _camera._alpha;
		betaAux = _camera._beta;
		rAux = _camera._r + (deltaY * 0.1f);

		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	_camera._coords.x = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	_camera._coords.z = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	_camera._coords.y = rAux * sin(betaAux * 3.14f / 180.0f);
}


void WorldCameraScript::_processMouseWheel(const InputHandler::MouseInfo& mouseInfo)
{
	if (mouseInfo.status != InputHandler::MouseStatus::SCROL)
		return;

	_camera._r -= mouseInfo.wheelDirection * 1.0f;
	if (_camera._r < 0.1f)
		_camera._r = 0.1f;

	_camera._coords.x = _camera._r * sin(_camera._alpha * 3.14f / 180.0f) * cos(_camera._beta * 3.14f / 180.0f);
	_camera._coords.z = _camera._r * cos(_camera._alpha * 3.14f / 180.0f) * cos(_camera._beta * 3.14f / 180.0f);
	_camera._coords.y = _camera._r * sin(_camera._beta * 3.14f / 180.0f);
}