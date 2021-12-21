#ifndef __ENGINE_GAME_SCRIPT_H__
#define __ENGINE_GAME_SCRIPT_H__




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
	friend class Scene;

};


#endif // !__ENGINE_GAME_SCRIPT_H__
