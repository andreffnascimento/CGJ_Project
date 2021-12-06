#include "camera.h"



Camera::Camera()
	: _alpha(200.0f), _beta(30.0f), _r(50.0f)
{
	_coords = Coords();
	_coords.x = _r * sin(_alpha * 3.14f / 180.0f) * cos(_beta * 3.14f / 180.0f);
	_coords.z = _r * cos(_alpha * 3.14f / 180.0f) * cos(_beta * 3.14f / 180.0f);
	_coords.y = _r * sin(_beta * 3.14f / 180.0f);
}