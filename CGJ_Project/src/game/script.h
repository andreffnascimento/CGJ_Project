#ifndef GAME_SCRIPT_H
#define GAME_SCRIPT_H



class Script
{
public:
	Script() {}



public:
	virtual void run() = 0;			// concrete script implements this method
};


#endif // !GAME_SCRIPT_H
