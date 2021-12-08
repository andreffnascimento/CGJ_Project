#ifndef __ENGINE_GAME_SCRIPT_H__
#define __ENGINE_GAME_SCRIPT_H__



class Script
{
public:
	Script() {}



public:
	virtual void run() = 0;			// concrete script implements this method
};


#endif // !__ENGINE_GAME_SCRIPT_H__
