#ifndef __MM3D_SCRIPTS_CAMERA_CAMERAMANAGER_H__
#define __MM3D_SCRIPTS_CAMERA_CAMERAMANAGER_H__


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


#endif // !__MM3D_SCRIPTS_CAMERA_CAMERAMANAGER_H__