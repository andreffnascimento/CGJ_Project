#include "physicsEngine.h"

#include "engine/math/transform.h"
#include "engine/scene/scene.h"
#include "engine/utils/coords.h"




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
		_calculateExpectedVelocity(rigidbody, ts);

	_processVelocityConstraints(rigidbody);
	_processSleepThreshold(rigidbody);

	if (rigidbody._sleeping)
		return;

	Entity entity = scene.getEntityById(entityId);
	TransformComponent& transform = entity.transform();
	Coords3f expectedPosition = transform.translation() + (rigidbody._velocity * ts);
	Transform::translateTo(entity, expectedPosition);

	rigidbody._force = Coords3f();
}




Coords3f PhysicsEngine::_calculateDragForce(const RigidbodyComponent& rigidbody) const
{
	Coords3f dragForce = rigidbody._velocity;
	dragForce *= PhysicsEngine::AIR_DENSITY * rigidbody._drag;
	if (rigidbody._velocity.sum() > rigidbody._dragThreshold)
		dragForce *= rigidbody._velocity;

	return dragForce;
}


void PhysicsEngine::_calculateExpectedVelocity(RigidbodyComponent& rigidbody, float ts) const
{
	Coords3f dragForce = _calculateDragForce(rigidbody);
	rigidbody._force -= dragForce;
	rigidbody._force *= (ts / rigidbody._mass);
	rigidbody._velocity += rigidbody._force;
}


void PhysicsEngine::_processVelocityConstraints(RigidbodyComponent& rigidbody) const
{
	rigidbody._velocity.x = !rigidbody._constraints[0] ? rigidbody._velocity.x : 0.0f;
	rigidbody._velocity.y = !rigidbody._constraints[1] ? rigidbody._velocity.y : 0.0f;
	rigidbody._velocity.z = !rigidbody._constraints[2] ? rigidbody._velocity.z : 0.0f;
}


void PhysicsEngine::_processSleepThreshold(RigidbodyComponent& rigidbody) const
{
	if (rigidbody._velocity.sum() > rigidbody._sleepThreshold)
		return;

	rigidbody._velocity = Coords3f();
	rigidbody._sleeping = true;
}
