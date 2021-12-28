#ifndef __mm3d_scripts_car_carMovementScript__
#define __mm3d_scripts_car_carMovementScript__


#include "MicroMachines3D/common/include.h"




class CarMovementScript : public Script
{

public:
	static constexpr float MIN_FORWARD_FORCE = 2000.0f;
	static constexpr float MAX_FORWARD_FORCE = 10000.0f;
	static constexpr float MIN_BACKWARDS_FORCE = -1000.0f;
	static constexpr float MAX_BACKWARDS_FORCE = -2000.0f;




private:
	const EventHandler* _eventHandler = nullptr;

	Entity _car = Entity();
	RigidbodyComponent* _rigidbody = nullptr;

	float _forwardForce = 0.0f;
	float _backwardsForce = 0.0f;



public:
	CarMovementScript() = delete;
	CarMovementScript(const CarMovementScript&) = default;
	CarMovementScript(Scene* scene) : Script(scene) {}
	~CarMovementScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_car = _scene->getEntityByTag("Car");
		_rigidbody = &_car.getComponent<RigidbodyComponent>();
		_rigidbody->setRotation({ 0.0f, 90.0f, 0.0f });
	}


	void onUpdate(float ts) override
	{
		_calculateForwardForce(ts);
		_calculateBackwardsForce(ts);

		float steeringVelocity = _calculateSteeringVelocity(ts);
		float travelForce = _forwardForce + _backwardsForce;

		_rigidbody->addRelativeForce({ 0.0f, 0.0f, travelForce});
		_rigidbody->setAngularVelocity({ 0.0f, steeringVelocity, 0.0f });
	}



private:
	void _calculateForwardForce(float ts)
	{
		if ((_eventHandler->keyState('Q').pressed() || _eventHandler->keyState('q').pressed()) && _forwardForce < MIN_FORWARD_FORCE)
			_forwardForce = MIN_FORWARD_FORCE;

		if (_eventHandler->keyState('Q').down() || _eventHandler->keyState('q').down())
			_forwardForce *= 63.0f * ts;
		else
			_forwardForce /= 65.0f * ts;

		if (_forwardForce < MIN_FORWARD_FORCE)
			_forwardForce = 0.0f;

		_forwardForce = std::min(_forwardForce, CarMovementScript::MAX_FORWARD_FORCE);
	}


	void _calculateBackwardsForce(float ts)
	{
		if ((_eventHandler->keyState('A').pressed() || _eventHandler->keyState('a').pressed()) && _backwardsForce > MIN_BACKWARDS_FORCE)
			_backwardsForce = MIN_BACKWARDS_FORCE;

		if (_eventHandler->keyState('A').down() || _eventHandler->keyState('a').down())
			_backwardsForce *= 62.0f * ts;
		else
			_backwardsForce /= 65.0f * ts;

		if (_backwardsForce > MIN_BACKWARDS_FORCE)
			_backwardsForce = 0.0f;
		
		_backwardsForce = std::max(_backwardsForce, CarMovementScript::MAX_BACKWARDS_FORCE);
	}

	float _calculateSteeringVelocity(float ts)
	{
		if (_eventHandler->keyState('O').down() || _eventHandler->keyState('o').down())
			return 2.0f * ts;

		if (_eventHandler->keyState('P').down() || _eventHandler->keyState('p').down())
			return -2.0f * ts;

		return 0.0f;
	}

};


#endif // !__mm3d_scripts_car_carMovementScript__