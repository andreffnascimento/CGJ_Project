#ifndef __engine_app_eventHandler__
#define __engine_app_eventHandler__


#include "engine/utils/coords.h"




class EventHandler
{

public:
	static const unsigned int N_KEYS = 127;




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




public:
	struct KeyState
	{
	private:
		bool _pressed = false;
		bool _updated = false;

	public:
		KeyState() = default;
		KeyState(const KeyState&) = default;
		KeyState(bool pressed, bool updated) : _pressed(pressed), _updated(updated) {}
		~KeyState() = default;

		inline bool down() const		{ return _pressed; }
		inline bool up() const			{ return !_pressed; }
		inline bool pressed() const		{ return _pressed && _updated; }
		inline bool released() const	{ return !_pressed && _updated; }

		friend class EventHandler;
	};


	struct MouseInfo
	{
		EventHandler::MouseStatus status = EventHandler::MouseStatus::NONE;
		WindowCoords coords = WindowCoords();
		int wheelDirection = 0;

		MouseInfo() = default;
		MouseInfo(const MouseInfo&) = default;
		~MouseInfo() = default;
	};




private:
	KeyState _keys[N_KEYS] = {};
	EventHandler::MouseInfo _mouseInfo = EventHandler::MouseInfo();
	



public:
	EventHandler();


public:
	void reset(bool resetKeyboard = false);


public:
	inline const EventHandler::KeyState& keyState(char keyCode) const	{ return _keys[(int)keyCode]; }
	inline const EventHandler::MouseInfo& getMouseInfo() const			{ return _mouseInfo; }


public:
	inline void pressKey(char keyCode)								{ _keys[(unsigned int)keyCode] = KeyState(true, true); }
	inline void releaseKey(char keyCode)							{ _keys[(unsigned int)keyCode] = KeyState(false, true); }
	inline void updateMouse(unsigned int x, unsigned int y)			{ _mouseInfo.coords = { x, y }; }
	inline void updateMouseStatus(EventHandler::MouseStatus status)	{ _mouseInfo.status = status; }
	inline void updateMouseWeel(int direction)						{ _mouseInfo.wheelDirection = direction; }

};


#endif // !__engine_app_eventHandler__