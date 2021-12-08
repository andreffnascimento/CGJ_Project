#ifndef __ENGINE_SCENE_ENTITY_H__
#define __ENGINE_SCENE_ENTITY_H__


#include <list>
#include <string>

#include "script.h"




class Entity
{

private:
	static unsigned int s_nextId;			// the id for the next entity

private:
	unsigned int _id;						// the id of the gameObject
	std::string _tag;						// gameObjects can have tags associated to them

	std::list<Script*> _scripts;			// game object contains a list of scripts




public:
	Entity();
	Entity(const char *tag);
	Entity(const std::string &tag);
	virtual ~Entity();					// to destroy the scripts in the concrete classes


public:
	virtual void update();
	virtual void render();


public:
	inline void addScript(Script *script)				{ _scripts.push_back(script); }
	inline bool hasTag(const char *tag) const			{ return _tag.compare(tag) == 0; }
	inline bool hasTag(const std::string &tag) const	{ return _tag.compare(tag) == 0; }

};



#endif // !__ENGINE_SCENE_ENTITY_H__
