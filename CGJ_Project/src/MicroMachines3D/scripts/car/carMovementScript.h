#ifndef __mm3d_scripts_car_carMovementScript__
#define __mm3d_scripts_car_carMovementScript__


#include "MicroMachines3D/common/include.h"




class CarMovementScript : public Script
{

public:
	static constexpr float MAX_FORWARD_FORCE = 10000.0f;
	static constexpr float MAX_BACKWARD_FORCE = -2000.0f;




private:
	const EventHandler* _eventHandler = nullptr;

	Entity _car = Entity();
	RigidbodyComponent* _rigidbody = nullptr;

	float _forwardForce = 0.0f;
	float _backwardsForce = 0.0f;
	float _steeringAngle = 0.0f;



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
		_steeringAngle = _car.transform().rotation().y;
	}


	void onUpdate(float ts) override
	{
		_calculateForwardForce(ts);
		_calculateBackwardsForce(ts);
		_calculateSteeringAngle(ts);

		float travelForce = _forwardForce + _backwardsForce;
		Coords3f carForce = _calculateCarForce(travelForce);

		_rigidbody->addForce(carForce);
		Transform::rotateTo(_car, { 0.0f, _steeringAngle, 0.0f });
	}



private:
	void _calculateForwardForce(float ts)
	{
		if ((_eventHandler->keyState('Q').pressed() || _eventHandler->keyState('q').pressed()) && _forwardForce < 2000.0f)
			_forwardForce = 2000.0f;

		if (_eventHandler->keyState('Q').down() || _eventHandler->keyState('q').down())
			_forwardForce *= 63.0f * ts;
		else
			_forwardForce /= 65.0f * ts;

		_forwardForce = std::min(_forwardForce, CarMovementScript::MAX_FORWARD_FORCE);
	}


	void _calculateBackwardsForce(float ts)
	{
		if ((_eventHandler->keyState('A').pressed() || _eventHandler->keyState('a').pressed()) && _backwardsForce > -1000.0f)
			_backwardsForce = -1000.0f;

		if (_eventHandler->keyState('A').down() || _eventHandler->keyState('a').down())
			_backwardsForce *= 62.0f * ts;
		else
			_backwardsForce /= 65.0f * ts;

		_backwardsForce = std::max(_backwardsForce, CarMovementScript::MAX_BACKWARD_FORCE);
	}

	void _calculateSteeringAngle(float ts)
	{
		_steeringAngle += _eventHandler->keyState('O').down() || _eventHandler->keyState('o').down() ? 45.0f * ts : 0.0f;
		_steeringAngle -= _eventHandler->keyState('P').down() || _eventHandler->keyState('p').down() ? 45.0f * ts : 0.0f;
		_steeringAngle = std::fmod(_steeringAngle, 360.0f);
	}


	Coords3f _calculateCarForce(float travelForce) const
	{
		float steeringAngleCos = (float)std::cos(toRadians(_steeringAngle));
		float steeringAngleSin = (float)std::sin(toRadians(_steeringAngle));
		return { travelForce * steeringAngleSin, 0.0f, travelForce * steeringAngleCos };
	}
};


#endif // !__mm3d_scripts_car_carMovementScript__