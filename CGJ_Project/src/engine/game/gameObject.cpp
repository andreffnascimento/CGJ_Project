#include "gameObject.h"




unsigned int GameObject::s_nextId = 0;




GameObject::GameObject()
	: _id(GameObject::s_nextId), _tag(""), _scripts(std::list<Script*>())
{
	GameObject::s_nextId++;
}

GameObject::GameObject(const char* tag)
	: _id(GameObject::s_nextId), _tag(tag), _scripts(std::list<Script*>())
{
	GameObject::s_nextId++;
}

GameObject::GameObject(const std::string& tag)
	: _id(GameObject::s_nextId), _tag(tag), _scripts(std::list<Script*>())
{
	GameObject::s_nextId++;
}


GameObject::~GameObject()
{
	for (auto& script : _scripts)
		delete script;
}




void GameObject::update()
{
	for (auto& script : _scripts)
		script->run();
}