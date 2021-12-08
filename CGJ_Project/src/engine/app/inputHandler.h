#ifndef __ENGINE_APP_INPUTHANDLER_H__
#define __ENGINE_APP_INPUTHANDLER_H__


#include "engine/utils/coords.h"




class InputHandler
{

public:
	enum class MouseStatus
	{
		NONE,
		LEFT_DOWN,
		RIGHT_DOWN,
		MOUSE_UP,
		MOVE,
		SCROL
	};


	struct MouseInfo
	{
		InputHandler::MouseStatus status;
		Coords2i coords;
		int wheelDirection;
	};




public:
	static const unsigned int N_KEYS = 127;


private:
	bool _pressedKeys[N_KEYS];
	InputHandler::MouseInfo _mouseInfo;
	



public:
	InputHandler();


public:
	void reset();


public:
	inline bool keyPressed(char keyCode) const						{ return _pressedKeys[(int)keyCode]; }
	inline const InputHandler::MouseInfo& getMouseInfo() const		{ return _mouseInfo; }


public:
	inline void pressKey(char keyCode)								{ _pressedKeys[(int)keyCode] = true; }
	inline void updateMouse(int x, int y)							{ _mouseInfo.coords = { x, y }; }
	inline void updateMouseStatus(InputHandler::MouseStatus status)	{ _mouseInfo.status = status; }
	inline void updateMouseWeel(int direction)						{ _mouseInfo.wheelDirection = direction; }

};


#endif // !__ENGINE_APP_INPUTHANDLER_H__