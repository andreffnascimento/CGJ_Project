#ifndef __mm3d_scripts_car_carMovementScript__
#define __mm3d_scripts_car_carMovementScript__


#include "MicroMachines3D/common/include.h"




class CarMovementScript : public Script
{

private:
	const InputHandler* _inputHandler = nullptr;

	RigidbodyComponent* _rigidbody = nullptr;

	float _forwardForce = 1000.0f;




public:
	CarMovementScript() = delete;
	CarMovementScript(const CarMovementScript&) = default;
	CarMovementScript(Scene* scene) : Script(scene) {}
	~CarMovementScript() = default;


public:
	void onCreate() override
	{
		_inputHandler = &Application::getInputHandler();
		_rigidbody = &_scene->getEntityByTag("Car").getComponent<RigidbodyComponent>();
	}


	void onUpdate(float ts) override
	{
		if (_inputHandler->keyPressed('Q') || _inputHandler->keyPressed('q'))
		{	
			_rigidbody->addForce({ _forwardForce, 0.0f, 0.0f });
			_forwardForce *= 1.01f;
		}
		else
		{
			_forwardForce = 1000.0f;
		}
	}

};


#endif // !__mm3d_scripts_car_carMovementScript__
