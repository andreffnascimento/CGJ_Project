#ifndef GAME_GAME_OBJECT_H
#define GAME_GAME_OBJECT_H


#include <list>

#include "script.h"



class GameObject
{
private:
	std::list<Script*> _scripts;			// game object contains a list of scripts


public:
	GameObject();
	virtual ~GameObject();


public:
	virtual void update();


public:
	inline void addScript(Script* script) { _scripts.push_back(script); }

};



#endif // !GAME_GAME_OBJECT_H
