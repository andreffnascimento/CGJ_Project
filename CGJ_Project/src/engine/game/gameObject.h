#ifndef __ENGINE_GAME_GAMEOBJECT_H__
#define __ENGINE_GAME_GAMEOBJECT_H__


#include <list>
#include <string>

#include "script.h"




class GameObject
{

private:
	static unsigned int s_nextId;			// the id for the next gameObject

private:
	unsigned int _id;						// the id of the gameObject
	std::string _tag;						// gameObjects can have tags associated to them
	std::list<Script*> _scripts;			// game object contains a list of scripts




public:
	GameObject();
	GameObject(const char *tag);
	GameObject(const std::string &tag);
	virtual ~GameObject();					// to destroy the scripts in the concrete classes


public:
	virtual void update();


public:
	inline void addScript(Script *script)				{ _scripts.push_back(script); }
	inline bool hasTag(const char *tag) const			{ return _tag.compare(tag) == 0; }
	inline bool hasTag(const std::string &tag) const	{ return _tag.compare(tag) == 0; }

};



#endif // !__ENGINE_GAME_GAMEOBJECT_H__
