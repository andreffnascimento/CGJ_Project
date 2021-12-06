#include "camera.h"



Camera::Camera()
	: _alpha(39.0f), _beta(51.0f), _r(10.0f)
{
	_coords = Coords();
	_coords.x = _r * sin(_alpha * 3.14f / 180.0f) * cos(_beta * 3.14f / 180.0f);
	_coords.z = _r * cos(_alpha * 3.14f / 180.0f) * cos(_beta * 3.14f / 180.0f);
	_coords.y = _r * sin(_beta * 3.14f / 180.0f);
}