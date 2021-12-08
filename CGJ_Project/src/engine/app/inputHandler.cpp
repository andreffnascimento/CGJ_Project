#include "inputHandler.h"

#include <string>




InputHandler::InputHandler()
{
	reset();
}




void InputHandler::reset()
{
	std::fill(std::begin(_pressedKeys), std::end(_pressedKeys), false);

	_mouseInfo = MouseInfo();
	_mouseInfo.coords = { 0, 0 };
	_mouseInfo.status = MouseStatus::NONE;
	_mouseInfo.wheelDirection = 0;
}