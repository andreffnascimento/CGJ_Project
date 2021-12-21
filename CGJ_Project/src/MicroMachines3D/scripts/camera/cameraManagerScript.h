#ifndef __mm3d_scripts_camera_cameraManagerScript_h__
#define __mm3d_scripts_camera_cameraManagerScript_h__


#include "MicroMachines3D/common/include.h"




class CameraManagerScript : public Script
{
private:
	InputHandler* _inputHandler = nullptr;

	Camera _camera1 = Camera();
	Camera _camera2 = Camera();
	Camera _camera3 = Camera();




public:
	CameraManagerScript() = delete;
	CameraManagerScript(const CameraManagerScript&) = default;
	CameraManagerScript(Scene* scene) : Script(scene) {}
	~CameraManagerScript() = default;


public:
	void onCreate() override
	{
		_inputHandler = &Application::getInputHandler();
		_camera1 = _scene->getEntityByTag("Camera1");
		_camera2 = _scene->getEntityByTag("Camera2");
		_camera3 = _scene->getEntityByTag("Camera3");
		_scene->setActiveCamera(_camera1);
	}


	void onUpdate(float ts) override
	{
		if (_inputHandler->keyPressed('1'))
			_scene->setActiveCamera(_camera1);

		if (_inputHandler->keyPressed('2'))
			_scene->setActiveCamera(_camera2);

		if (_inputHandler->keyPressed('3'))
			_scene->setActiveCamera(_camera3);
	}
};


#endif // !__mm3d_scripts_camera_cameraManagerScript_h__