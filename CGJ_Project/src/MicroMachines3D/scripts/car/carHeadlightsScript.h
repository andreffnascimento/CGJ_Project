#ifndef __mm3d_scripts_car_carHeadlightsScript__
#define __mm3d_scripts_car_carHeadlightsScript__


#include "MicroMachines3D/common/include.h"




class CarHeadlightsScript : public Script
{

private:
	const InputHandler* _inputHandler = nullptr;

	MeshComponent* _frontMesh = nullptr;
	MeshComponent* _backMesh  = nullptr;

	bool _lightsOn = false;




public:
	CarHeadlightsScript() = delete;
	CarHeadlightsScript(const CarHeadlightsScript&) = default;
	CarHeadlightsScript(Scene* scene) : Script(scene) {}
	~CarHeadlightsScript() = default;


public:
	void onCreate() override
	{
		_inputHandler = &Application::getInputHandler();
		_frontMesh = &_scene->getEntityByTag("Car:headlight-frontLeft").getComponent<MeshComponent>();
		_backMesh  = &_scene->getEntityByTag("Car:headlight-backLeft").getComponent<MeshComponent>();
	}


	void onUpdate(float ts) override
	{
		if (_inputHandler->keyPressed('H') || _inputHandler->keyPressed('h'))
		{
			_lightsOn = !_lightsOn;

			if (_lightsOn)
			{
				_frontMesh->setMaterial(CAR_HEADLIGHT_FRONT_ON_MATERIAL);
				_backMesh->setMaterial(CAR_HEADLIGHT_BACK_ON_MATERIAL);
			}
			else
			{
				_frontMesh->setMaterial(CAR_HEADLIGHT_FRONT_OFF_MATERIAL);
				_backMesh->setMaterial(CAR_HEADLIGHT_BACK_OFF_MATERIAL);
			}
		}
	}

};


#endif // !__mm3d_scripts_car_carHeadlightsScript__
