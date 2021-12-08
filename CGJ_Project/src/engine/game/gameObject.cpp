#include "gameObject.h"




GameObject::GameObject()
	: _scripts(std::list<Script*>()), _tag(0)
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


void GameObject::render()
{
	// empty by default
}


GameObject::Type GameObject::getType() const
{
	return GameObject::Type::GAME_OBJECT;
}
