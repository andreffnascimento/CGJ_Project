/*#ifndef __MM3D_SCRIPS_CAMERA_ORBITTARGETSCRIPT_H__
#define __MM3D_SCRIPS_CAMERA_ORBITTARGETSCRIPT_H__


#include "engine/app/inputHandler.h"
#include "engine/game/script.h"
#include "engine/"
#include "engine/gameObjects/components/camera.h"



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
	const InputHandler &_input;

	int _startX;
	int _startY;
	TrackingStatus _trackingStatus;



public:
	WorldCameraScript(Camera &camera, const InputHandler &input);


public:
	void run() override;


private:
	void _processMouseClick(const InputHandler::MouseInfo& mouseInfo);
	void _processMouseMovement(const InputHandler::MouseInfo& mouseInfo);
	void _processMouseWheel(const InputHandler::MouseInfo& mouseInfo);
};


#endif // !__MM3D_SCRIPS_CAMERA_ORBITTARGETSCRIPT_H_`*/
