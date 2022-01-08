#ifndef __mm3d_scripts_oranges_orangeMovementScript__
#define __mm3d_scripts_oranges_orangeMovementScript__


#include "MicroMachines3D/common/include.h"




class OrangeMovementScript : public Script 
{

public:
	static constexpr float BASE_MOVEMENT_SPEED = 500.0f;
	static constexpr float MAX_MOVEMENT_SPEED = 5000.0f;
	static constexpr float MOVEMENT_SPEED_INCREMENT = 100.0f;


private:
	std::string _tag;
	Entity _orange = Entity();
	RigidbodyComponent* _rigidbody = nullptr;

	Coords3f _movementDirection = Coords3f({ 0, 0, 0 });
	float _movementSpeed = 0;


public:
	OrangeMovementScript() = delete;
	OrangeMovementScript(const OrangeMovementScript&) = default;
	OrangeMovementScript(Scene * scene, std::string tag, Coords3f movementDirection) 
		: Script(scene) 
	{ 
		_tag = tag;
		setMovementDirection(movementDirection);
	}
	~OrangeMovementScript() = default;

	
public:
	void onCreate() 
	{
		_orange = _scene->getEntityByTag(_tag);
		_rigidbody = &_orange.getComponent<RigidbodyComponent>();
		_movementSpeed = OrangeMovementScript::BASE_MOVEMENT_SPEED;
	}

	void setMovementDirection(Coords3f direction) 
	{
		_movementDirection = direction;

		float velocity = _movementDirection.normalize() * _movementSpeed;
		_rigidbody->setVelocity(velocity);
	}

	void respawnAtPosition(Coords3f position)
	{
		Transform::translate(_orange, position);

		_movementSpeed += OrangeMovementScript::MOVEMENT_SPEED_INCREMENT;
	}


};

#endif // !__mm3d_scripts_oranges_orangeMovementScript__