#include "inputHandler.h"


InputHandler::InputHandler()
{
	resetInput();	
}





void InputHandler::resetInput()
{
	std::fill(std::begin(_pressedKeys), std::end(_pressedKeys), false);

	_mouseStartCoords = { 0, 0 };
	_mouseStatus = MouseStatus::NONE;
	_mouseWeelDirection = 0;
}





void InputHandler::updateMouse(int x, int y)
{
	_mouseStartCoords = { x, y };
}

void InputHandler::updateMouse(MouseStatus status, int x, int y)
{
	_mouseStatus = status;
	_mouseStartCoords = { x, y };
}

void InputHandler::updateMouseWeel(int direction)
{
}
