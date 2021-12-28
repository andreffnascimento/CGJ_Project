#ifndef __engine_app_inputHandler__
#define __engine_app_inputHandler__


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
		WindowCoords coords;
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
	void reset(bool resetKeyboard = false);


public:
	inline bool keyPressed(char keyCode) const						{ return _pressedKeys[(unsigned int)keyCode]; }
	inline const InputHandler::MouseInfo& getMouseInfo() const		{ return _mouseInfo; }


public:
	inline void pressKey(char keyCode)								{ _pressedKeys[(unsigned int)keyCode] = true; }
	inline void releaseKey(char keyCode)							{ _pressedKeys[(unsigned int)keyCode] = false; }
	inline void updateMouse(unsigned int x, unsigned int y)			{ _mouseInfo.coords = { x, y }; }
	inline void updateMouseStatus(InputHandler::MouseStatus status)	{ _mouseInfo.status = status; }
	inline void updateMouseWeel(int direction)						{ _mouseInfo.wheelDirection = direction; }

};


#endif // !__engine_app_inputHandler__