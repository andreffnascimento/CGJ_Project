#include "gameObject.h"



GameObject::GameObject()
	: _scripts(std::list<Script*>())
{
	// empty
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