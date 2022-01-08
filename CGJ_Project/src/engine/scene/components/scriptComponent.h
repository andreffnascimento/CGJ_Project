#ifndef __engine_scene_components_scriptComponent__
#define __engine_scene_components_scriptComponent__


#include <list>
#include <memory>

#include "engine/scene/script.h"




struct ScriptComponent
{
private:
	std::list<std::shared_ptr<Script>> _scripts = std::list<std::shared_ptr<Script>>();

public:
	ScriptComponent() = default;
	ScriptComponent(const ScriptComponent&) = default;
	ScriptComponent(const std::shared_ptr<Script>& script) { _scripts.push_back(script); }
	~ScriptComponent() = default;

	Script* getScriptByTag(const std::string& tag);

	void addScriptInstance(const std::shared_ptr<Script>& script) { _scripts.push_back(script); }

	void onCreate() const;
	void onUpdate(float ts) const;
	void onFixedUpdate(float ts) const;
};


#endif // !__engine_scene_components_scriptComponent__