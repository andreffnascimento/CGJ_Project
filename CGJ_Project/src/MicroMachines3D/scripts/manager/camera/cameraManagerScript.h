#ifndef __mm3d_scripts_manager_camera_cameraManagerScript__
#define __mm3d_scripts_manager_camera_cameraManagerScript__


#include "MicroMachines3D/common/include.h"




class CameraManagerScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	CameraEntity _camera1 = CameraEntity();
	CameraEntity _camera2 = CameraEntity();
	CameraEntity _camera3 = CameraEntity();




public:
	CameraManagerScript() = delete;
	CameraManagerScript(const CameraManagerScript&) = default;
	CameraManagerScript(Scene* scene) : Script(scene) {}
	~CameraManagerScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_camera1 = _scene->getEntityByTag("Camera1");
		_camera2 = _scene->getEntityByTag("Camera2");
		_camera3 = _scene->getEntityByTag("Camera3");
		_scene->setActiveCamera(_camera1);
	}


	void onUpdate(float ts) override
	{
		if (_eventHandler->keyPressed('1'))
			_scene->setActiveCamera(_camera1);

		if (_eventHandler->keyPressed('2'))
			_scene->setActiveCamera(_camera2);

		if (_eventHandler->keyPressed('3'))
			_scene->setActiveCamera(_camera3);
	}

};


#endif // !__mm3d_scripts_manager_camera_cameraManagerScript__