#ifndef APP_INPUT_HANDLER_H
#define APP_INPUT_HANDLER_H


#include <string>



class InputHandler
{
public:
	enum class MouseStatus
	{
		NONE,
		LEFT_CLICK,
		RIGHT_CLICK
	};
	
	struct Coords
	{
		int x;
		int y;
	};


public:
	static const unsigned int N_KEYS = 127;


private:
	bool _pressedKeys[N_KEYS];

	MouseStatus _mouseStatus;
	Coords _mouseStartCoords;
	int _mouseWeelDirection;




public:
	InputHandler();


private:
	void resetInput();


	// keyboard methods
public:
	void inline pressKey(char keyCode)		{ _pressedKeys[(int)keyCode] = true; }
	bool inline keyPressed(char keyCode)	{ return _pressedKeys[(int)keyCode]; }


	// mouse methods
public:
	void updateMouse(int x, int y);
	void updateMouse(MouseStatus status, int x, int y);
	void updateMouseWeel(int direction);

};


#endif // !APP_INPUT_HANDLER_H
