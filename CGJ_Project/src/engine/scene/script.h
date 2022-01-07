#ifndef __engine_scene_script__
#define __engine_scene_script__


class Scene;




class Script
{

protected:
	Scene* _scene;


private:
	std::string _tag = "Script";




public:
	Script() = delete;
	Script(const Script&) = default;
	Script(Scene* scene) : _scene(scene) {}
	Script(Scene* scene, const char* tag) : _scene(scene), _tag(tag) {}
	virtual ~Script() = default;


protected:
	virtual void onCreate()			{}
	virtual void onUpdate(float ts) {}


private:
	friend inline bool operator==(const Script& script, const std::string& tag) { return script._tag == tag; }




public:
	friend struct ScriptComponent;

};


#endif // !__engine_scene_script__