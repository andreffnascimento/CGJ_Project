#ifndef __ENGINE_GAME_GAMEOBJECT_H__
#define __ENGINE_GAME_GAMEOBJECT_H__


#include <list>

#include "script.h"




class GameObject
{

public:
	enum class Type
	{
		GAME_OBJECT,
		CAMERA,
		LIGHT,
		RENDERABLE
	};




private:
	std::list<Script*> _scripts;			// game object contains a list of scripts
	unsigned int _tag;						// gameObjects can have tags associated to them




public:
	GameObject();
	virtual ~GameObject();					// to destroy the scripts in the concrete classes


public:
	virtual void update();
	virtual void render();

	virtual GameObject::Type getType() const;


public:
	inline void addScript(Script *script)		{ _scripts.push_back(script); }
	inline bool hasTag(unsigned int tag) const	{ return _tag == tag; }

};



#endif // !__ENGINE_GAME_GAMEOBJECT_H__
