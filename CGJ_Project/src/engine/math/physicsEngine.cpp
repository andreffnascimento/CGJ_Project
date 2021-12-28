#include "physicsEngine.h"

#include <unordered_map>

#include "engine/scene/scene.h"
#include "engine/utils/coords.h"




void PhysicsEngine::run(const Scene& scene, float ts) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent> _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& iterator : _rigidbodyComponents)
	{
		EntityHandle entityId = iterator.first;
		RigidbodyComponent& rigidbody = iterator.second;

		if (!rigidbody._sleeping)	// ignore rigidbodies that haven't move
			_processRigidbody(entityId, rigidbody, ts);	
	}
}




void PhysicsEngine::_processRigidbody(EntityHandle entityId, RigidbodyComponent& rigidbody, float ts) const
{
	if (rigidbody._type == RigidbodyComponent::RigidbodyType::DYNAMIC)
		_calculateExpectedVelocity(rigidbody, ts);


}




void PhysicsEngine::_calculateExpectedVelocity(RigidbodyComponent& rigidbody, float ts) const
{
	Coords3f dragForce = _calculateDragForce(rigidbody);
	rigidbody._force += dragForce;
	rigidbody._force *= (ts / rigidbody._mass);
	rigidbody._velocity += rigidbody._force;
}


Coords3f PhysicsEngine::_calculateDragForce(const RigidbodyComponent& rigidbody) const
{
	// we ignore the contact surface area for this calculations for performance reasons
	Coords3f dragForce = rigidbody._velocity;
	dragForce *= rigidbody._velocity;
	dragForce *= PhysicsEngine::AIR_DENSITY * rigidbody._drag;
	return dragForce;
}
