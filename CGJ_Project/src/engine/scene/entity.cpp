#include "entity.h"




unsigned int Entity::s_nextId = 0;




Entity::Entity()
	: _id(Entity::s_nextId), _tag(""), _scripts(std::list<Script*>())
{
	Entity::s_nextId++;
}

Entity::Entity(const char* tag)
	: _id(Entity::s_nextId), _tag(tag), _scripts(std::list<Script*>())
{
	Entity::s_nextId++;
}

Entity::Entity(const std::string& tag)
	: _id(Entity::s_nextId), _tag(tag), _scripts(std::list<Script*>())
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


void Entity::render()
{
	// empty
}
