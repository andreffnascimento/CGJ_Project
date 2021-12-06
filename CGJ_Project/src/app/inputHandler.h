#ifndef APP_INPUT_HANDLER_H
#define APP_INPUT_HANDLER_H


#include <string>



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
	
	struct Coords
	{
		int x;
		int y;
	};

	struct MouseInfo
	{
		MouseStatus status;
		Coords coords;
		int wheelDirection;
	};



public:
	static const unsigned int N_KEYS = 127;



private:
	bool _pressedKeys[N_KEYS];
	MouseInfo _mouseInfo;
	


public:
	InputHandler();


public:
	void reset();


	// keyboard methods
public:
	void inline pressKey(char keyCode)			{ _pressedKeys[(int)keyCode] = true; }
	bool inline keyPressed(char keyCode) const	{ return _pressedKeys[(int)keyCode]; }


	// mouse methods
public:
	inline void updateMouse(int x, int y)				{ _mouseInfo.coords = { x, y }; }
	inline void updateMouseStatus(MouseStatus status)	{ _mouseInfo.status = status; }
	inline void updateMouseWeel(int direction)			{ _mouseInfo.wheelDirection = direction; }


public:
	inline const MouseInfo& getMouseInfo() const { return _mouseInfo; }

};


#endif // !APP_INPUT_HANDLER_H
