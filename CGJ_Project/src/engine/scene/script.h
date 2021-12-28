#ifndef __engine_scene_script__
#define __engine_scene_script__




class Script
{
protected:
	Scene* _scene;




public:
	Script() = delete;
	Script(const Script&) = default;
	Script(Scene* scene) : _scene(scene) {}
	virtual ~Script() = default;


protected:
	virtual void onCreate()			{}
	virtual void onUpdate(float ts) {}


public:
	friend struct ScriptComponent;

};


#endif // !__engine_scene_script__