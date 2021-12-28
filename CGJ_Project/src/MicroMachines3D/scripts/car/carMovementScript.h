#ifndef __mm3d_scripts_car_carMovementScript__
#define __mm3d_scripts_car_carMovementScript__


#include "MicroMachines3D/common/include.h"




class CarMovementScript : public Script
{

private:
	const EventHandler* _eventHandler = nullptr;

	RigidbodyComponent* _rigidbody = nullptr;

	float _forwardForce = 0.0f;




public:
	CarMovementScript() = delete;
	CarMovementScript(const CarMovementScript&) = default;
	CarMovementScript(Scene* scene) : Script(scene) {}
	~CarMovementScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_rigidbody = &_scene->getEntityByTag("Car").getComponent<RigidbodyComponent>();
	}


	void onUpdate(float ts) override
	{
		if ((_eventHandler->keyState('Q').pressed() || _eventHandler->keyState('q').pressed()) && _forwardForce < 1000.0f)
			_forwardForce = 1000.0f;

		_forwardForce *= (_eventHandler->keyState('Q').down() || _eventHandler->keyState('q').down() ? 1.02f : 0.9f);
		_rigidbody->addForce({ _forwardForce, 0.0f, 0.0f });
	}

};


#endif // !__mm3d_scripts_car_carMovementScript__