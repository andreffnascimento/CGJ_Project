#ifndef __ENGINE_SCENE_ENTITY_H__
#define __ENGINE_SCENE_ENTITY_H__


#include <list>
#include <string>

#include "engine/scene/script.h"


class Scene;



class Entity
{

private:
	static unsigned int s_nextId;			// the id for the next entity

private:
	unsigned int _id;						// the id of the gameObject
	std::string _tag;						// gameObjects can have tags associated to them
	std::list<Script*> _scripts;			// game object contains a list of scripts
	
	const Scene *_scene;					// a reference to the scene where the entity is stored
	const Entity *_parent;					// the entity's parent (nullptr if none)
	std::list<Entity*> _children;			// the children of the entity




public:
	Entity();
	virtual ~Entity();						// to destroy the scripts in the concrete classes


public:
	virtual void update();
	void addChild(Entity *child);


public:
	inline void addScript(Script *script)				{ _scripts.push_back(script); }
	inline void addTag(const char* tag)					{ _tag = std::string(tag); }
	inline void addTag(const std::string &tag)			{ _tag = std::string(tag); }
	inline bool hasTag(const char *tag) const			{ return _tag.compare(tag) == 0; }
	inline bool hasTag(const std::string &tag) const	{ return _tag.compare(tag) == 0; }


public:
	friend class Scene;

};



#endif // !__ENGINE_SCENE_ENTITY_H__