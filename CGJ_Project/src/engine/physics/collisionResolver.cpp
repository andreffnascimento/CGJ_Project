#include "collisionResolver.h"

#include "engine/scene/components.h"




void CollisionResolver::reset()
{
	_collisions.clear();
	_impulseForces.clear();
}


void CollisionResolver::addCollision(const AABBColliderComponent& otherCollider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse)
{
	_collisions.emplace_back(otherCollider, collisionNormal, relativeVelocity, impulse);
}


void CollisionResolver::processCollisions(const AABBColliderComponent& collider)
{
	for (const auto& collision : _collisions)
		if (collider.rigidbody()->type() != RigidbodyComponent::RigidbodyType::STATIC)
			_processCollision(collider, collision);
}


void CollisionResolver::updateVelocity(Coords3f& velocity)
{
	for (const auto& force : _impulseForces)
		velocity += force.value();
}




bool CollisionResolver::ignoreCollision(const RigidbodyComponent& rigidbody)
{
	return rigidbody.sleeping() || rigidbody.type() == RigidbodyComponent::RigidbodyType::STATIC;
}




void CollisionResolver::_processCollision(const AABBColliderComponent& collider, const Collision& collision)
{
	collider.collisionResolver()->_impulseForces.emplace_back(Force::ForceType::LINEAR, collision.collisionNormal() * collision.impulse());
}