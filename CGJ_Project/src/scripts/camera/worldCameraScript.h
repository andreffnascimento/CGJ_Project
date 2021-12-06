#ifndef SCRIPTS_CAMERA_WORLD_CAMERA_SCRIPT_H
#define SCRIPTS_CAMERA_WORLD_CAMERA_SCRIPT_H


#include "app/inputHandler.h"
#include "game/script.h"
#include "game/camera.h"



class WorldCameraScript : public Script
{

private:
	enum class TrackingStatus
	{
		NONE,
		MOVE,
		ZOOM
	};



private:
	Camera &_camera;
	InputHandler& _input;

	int _startX;
	int _startY;
	TrackingStatus _trackingStatus;



public:
	WorldCameraScript(Camera &camera, InputHandler &input);


public:
	void run() override;


private:
	void _processMouseClick(const InputHandler::MouseInfo& mouseInfo);
	void _processMouseMovement(const InputHandler::MouseInfo& mouseInfo);
	void _processMouseWheel(const InputHandler::MouseInfo& mouseInfo);
};


#endif // !SCRIPTS_CAMERA_WORLD_CAMERA_SCRIPT_H
