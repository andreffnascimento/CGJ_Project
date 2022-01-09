#ifndef __mm3d_scripts_car_carMovementScript__
#define __mm3d_scripts_car_carMovementScript__


#include "MicroMachines3D/common/include.h"




class CarMovementScript : public Script
{

private:
	static constexpr float ENGINE_ACCELERATION = 5000.0f;
	static constexpr float ENGINE_DEACCELERATION = 20000.0f;
	static constexpr float BASE_FORWARD_FORCE = 2000.0f;
	static constexpr float MAX_FORWARD_FORCE = 10000.0f;
	static constexpr float BASE_BACKWARDS_FORCE = 2000.0f;
	static constexpr float MAX_BACKWARDS_FORCE = 4000.0f;
	static constexpr float STEERING_FORCE = 40000.0f;




private:
	const EventHandler* _eventHandler = nullptr;

	Entity _car = Entity();
	RigidbodyComponent* _rigidbody = nullptr;

	float _forwardForce = 0.0f;
	float _backwardsForce = 0.0f;



public:
	CarMovementScript() = delete;
	CarMovementScript(const CarMovementScript&) = default;
	CarMovementScript(Scene* scene) : Script(scene, "CarMovementScript") {}
	~CarMovementScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_car = _scene->getEntityByTag("Car");
		_rigidbody = &_car.getComponent<RigidbodyComponent>();
	}


	void onFixedUpdate(float ts) override
	{
		if (std::abs(_rigidbody->velocity().y) > 2.0f)	// car is not on top of the table
			return;
		
		_calculateForwardForce(ts);
		_calculateBackwardsForce(ts);

		float travelForce = _forwardForce - _backwardsForce;
		float steeringForce = _calculateSteeringForce(ts);

		_rigidbody->addRelativeForce({ 0.0f, 0.0f, travelForce });
		_rigidbody->addAngularForce({ 0.0f, steeringForce, 0.0f });
	}




public:
	inline float travelForce() const
	{
		return _forwardForce - _backwardsForce;
	}




private:
	void _calculateForwardForce(float ts)
	{
		if ((_eventHandler->keyState('Q').pressed() || _eventHandler->keyState('q').pressed()) && _forwardForce < CarMovementScript::BASE_FORWARD_FORCE)
			_forwardForce = CarMovementScript::BASE_FORWARD_FORCE;

		if (_eventHandler->keyState('Q').down() || _eventHandler->keyState('q').down())
		{
			_forwardForce += CarMovementScript::ENGINE_ACCELERATION * ts;
		}
		else
		{
			_forwardForce -= CarMovementScript::ENGINE_DEACCELERATION * ts;
			if (_forwardForce < CarMovementScript::BASE_FORWARD_FORCE)
				_forwardForce = 0.0f;
		}

		_forwardForce = std::min(_forwardForce, CarMovementScript::MAX_FORWARD_FORCE);
	}


	void _calculateBackwardsForce(float ts)
	{
		if ((_eventHandler->keyState('A').pressed() || _eventHandler->keyState('a').pressed()) && _backwardsForce < CarMovementScript::BASE_BACKWARDS_FORCE)
			_backwardsForce = CarMovementScript::BASE_BACKWARDS_FORCE;

		if (_eventHandler->keyState('A').down() || _eventHandler->keyState('a').down())
		{
			_backwardsForce += CarMovementScript::ENGINE_ACCELERATION * ts;
		}
		else
		{
			_backwardsForce -= CarMovementScript::ENGINE_DEACCELERATION * ts;
			if (_backwardsForce < CarMovementScript::BASE_BACKWARDS_FORCE)
				_backwardsForce = 0.0f;
		}

		_backwardsForce = std::min(_backwardsForce, CarMovementScript::MAX_BACKWARDS_FORCE);
	}

	float _calculateSteeringForce(float ts)
	{
		float steeringForce = 0.0f;
		Coords3f velocity = Coords3f({ _rigidbody->velocity().x, 0.0f, _rigidbody->velocity().z });

		if (_eventHandler->keyState('O').down() || _eventHandler->keyState('o').down())
			steeringForce += CarMovementScript::STEERING_FORCE * velocity.length();

		if (_eventHandler->keyState('P').down() || _eventHandler->keyState('p').down())
			steeringForce -= CarMovementScript::STEERING_FORCE * velocity.length();

		return steeringForce;
	}

};


#endif // !__mm3d_scripts_car_carMovementScript__