#include "eventHandler.h"




EventHandler::EventHandler()
{
	reset(true);
}




void EventHandler::reset(bool resetKeyboard)
{
	if (resetKeyboard)
		for (int i = 0; i < EventHandler::N_KEYS; i++)
			_keys[i]._pressed = false;

	for (int i = 0; i < EventHandler::N_KEYS; i++)
		_keys[i]._updated = false;

	_mouseInfo = MouseInfo();
	_mouseInfo.coords = { 0, 0 };
	_mouseInfo.status = MouseStatus::NONE;
	_mouseInfo.wheelDirection = 0;
}