#include "entity.h"




unsigned int Entity::s_nextId = 0;




Entity::Entity()
	: _id(Entity::s_nextId), _tag(""), _scripts(std::list<Script*>()), 
	  _scene(nullptr), _parent(nullptr), _children(std::list<Entity*>())
{
	Entity::s_nextId++;
}


Entity::~Entity()
{
	for (auto& script : _scripts)
		delete script;
}




void Entity::update()
{
	for (auto& script : _scripts)
		script->run();
}


void Entity::addChild(Entity* child)
{
	_children.push_back(child);
	child->_parent = this;
}
