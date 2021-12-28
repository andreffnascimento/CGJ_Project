#ifndef __engine_app_eventHandler__
#define __engine_app_eventHandler__


#include "engine/utils/coords.h"




class EventHandler
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
		EventHandler::MouseStatus status;
		WindowCoords coords;
		int wheelDirection;
	};




public:
	static const unsigned int N_KEYS = 127;


private:
	bool _pressedKeys[N_KEYS];
	EventHandler::MouseInfo _mouseInfo;
	



public:
	EventHandler();


public:
	void reset();


public:
	inline bool keyPressed(char keyCode) const						{ return _pressedKeys[(int)keyCode]; }
	inline const EventHandler::MouseInfo& getMouseInfo() const		{ return _mouseInfo; }


public:
	inline void pressKey(char keyCode)								{ _pressedKeys[(int)keyCode] = true; }
	inline void updateMouse(unsigned int x, unsigned int y)			{ _mouseInfo.coords = { x, y }; }
	inline void updateMouseStatus(EventHandler::MouseStatus status)	{ _mouseInfo.status = status; }
	inline void updateMouseWeel(int direction)						{ _mouseInfo.wheelDirection = direction; }

};


#endif // !__engine_app_eventHandler__