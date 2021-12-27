#include "physicsEngine.h"

#include <unordered_map>

#include "engine/scene/scene.h"
#include "engine/utils/coords.h"




void PhysicsEngine::run(const Scene& scene) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent> _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& iterator : _rigidbodyComponents)
	{
		EntityHandle entityId = iterator.first;
		RigidbodyComponent& rigidbody = iterator.second;
		
		if (!rigidbody._forces.empty())				// ignore rigidbodies that haven't been modified by a force
			processRigidbody(entityId, rigidbody);
	}
}




void PhysicsEngine::processRigidbody(EntityHandle entityId, RigidbodyComponent& rigidbody) const
{
	Coords3f linearForce = Coords3f();
	Coords3f angularForce = Coords3f();
	combineForces(rigidbody, linearForce, angularForce);
	addGravityForce(rigidbody, linearForce);
}




void PhysicsEngine::combineForces(RigidbodyComponent& rigidbody, Coords3f& linearForce, Coords3f& angularForce) const
{
	for (const auto& force : rigidbody._forces)
	{
		if (force.type() == Force::ForceType::LINEAR)
			linearForce += force.force();
		else
			angularForce += force.force();
	}

	rigidbody._forces.clear();
}


void PhysicsEngine::addGravityForce(const RigidbodyComponent& rigidbody, Coords3f& linearForce) const
{
	if (rigidbody.gravityEnabled())
		linearForce.y -= PhysicsEngine::GRAVITY;
}
