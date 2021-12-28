#include "physicsEngine.h"

#include <cmath>

#include "engine/math/transform.h"
#include "engine/scene/scene.h"
#include "engine/utils/coords.h"




void PhysicsEngine::rotateVector(Coords3f& vector, const Coords3f& rotation)
{
	if (rotation.x != 0.0f)
		PhysicsEngine::rotateVectorOnAxis(vector.z, vector.y, rotation.x);

	if (rotation.y != 0.0f)
		PhysicsEngine::rotateVectorOnAxis(vector.x, vector.z, rotation.y);

	if (rotation.z != 0.0f)
		PhysicsEngine::rotateVectorOnAxis(vector.y, vector.z, rotation.z);
}


void PhysicsEngine::rotateVectorOnAxis(float& coord1, float& coord2, float angle)
{
	float cosAngle = std::cos(toRadians(angle));
	float sinAngle = std::sin(toRadians(angle));
	float oldCoord1 = coord1;
	float oldCoord2 = coord2;
	coord1 = oldCoord1 * cosAngle + oldCoord2 * sinAngle;
	coord2 = oldCoord2 * cosAngle - oldCoord1 * sinAngle;
}


Coords3f PhysicsEngine::calculateDragForce(const Coords3f& velocity, float drag, float dragThreshold)
{
	Coords3f dragForce = velocity;
	dragForce *= PhysicsEngine::AIR_DENSITY * drag;
	if (velocity.length() > dragThreshold)
		dragForce *= velocity;
	else
		dragForce *= PhysicsEngine::DRAG_SLOW_CONSTANT;

	dragForce *= velocity.normalized();
	return dragForce;
}




void PhysicsEngine::run(const Scene& scene, float ts) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent>& _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& iterator : _rigidbodyComponents)
	{
		EntityHandle entityId = iterator.first;
		RigidbodyComponent& rigidbody = iterator.second;

		if (!rigidbody._sleeping)	// ignore rigidbodies that haven't move
			_processRigidbody(scene, entityId, rigidbody, ts);	
	}
}




void PhysicsEngine::_processRigidbody(const Scene& scene, EntityHandle entityId, RigidbodyComponent& rigidbody, float ts) const
{

	if (rigidbody._type == RigidbodyComponent::RigidbodyType::DYNAMIC)
	{
		Coords3f linearForce = Coords3f();
		Coords3f angularForce = Coords3f();
		_combineForces(rigidbody, linearForce, angularForce);
		_calculateExpectedAngularVelocity(rigidbody, angularForce, ts);
		Coords3f rotation = _calculateExpectedRotation(rigidbody, ts);
		_calculateExpectedVelocity(rigidbody, linearForce, ts, rotation);
	}

	_processSleepThreshold(rigidbody);

	if (rigidbody._sleeping)
		return;
	
	Entity entity = scene.getEntityById(entityId);	
	TransformComponent& transform = entity.transform();
	Coords3f expectedPosition = transform.translation() + (rigidbody._velocity * ts);
	Transform::translateTo(entity, expectedPosition);
	Transform::rotateTo(entity, rigidbody._rotation);
}




Coords3f PhysicsEngine::_calculateExpectedRotation(RigidbodyComponent& rigidbody, float ts) const
{
	Coords3f rotation = rigidbody._angularVelocity / ts;
	rigidbody._rotation += rotation;
	rigidbody._rotation.x = std::fmod(rigidbody._rotation.x, 360.0f);
	rigidbody._rotation.y = std::fmod(rigidbody._rotation.y, 360.0f);
	rigidbody._rotation.z = std::fmod(rigidbody._rotation.z, 360.0f);
	return rotation;
}


void PhysicsEngine::_combineForces(RigidbodyComponent& rigidbody, Coords3f& linearForce, Coords3f& angularForce) const
{
	for (const auto& force : rigidbody._forces)
	{
		if (force.type() == Force::ForceType::LINEAR)
			linearForce += force.value();
		else
			angularForce += force.value();
	}

	rigidbody._forces.clear();
}


void PhysicsEngine::_calculateExpectedAngularVelocity(RigidbodyComponent& rigidbody, Coords3f& angularForce, float ts) const
{
	angularForce -= PhysicsEngine::calculateDragForce(rigidbody._angularVelocity, rigidbody._angularDrag, rigidbody._dragThreshold);
	angularForce *= (ts / rigidbody._mass);
	rigidbody._angularVelocity += angularForce;
}


void PhysicsEngine::_calculateExpectedVelocity(RigidbodyComponent& rigidbody, Coords3f& linearForce, float ts, const Coords3f& rotation) const
{
	linearForce -= PhysicsEngine::calculateDragForce(rigidbody._velocity, rigidbody._drag, rigidbody._dragThreshold);
	linearForce *= (ts / rigidbody._mass);
	PhysicsEngine::rotateVector(linearForce, rotation);
	PhysicsEngine::rotateVector(rigidbody._velocity, rotation);
	rigidbody._velocity += linearForce;
}


void PhysicsEngine::_processSleepThreshold(RigidbodyComponent& rigidbody) const
{
	if (rigidbody._velocity.length() > rigidbody._sleepThreshold)
		return;

	rigidbody._velocity = Coords3f();
	rigidbody._angularVelocity = Coords3f();
	rigidbody._sleeping = true;
}