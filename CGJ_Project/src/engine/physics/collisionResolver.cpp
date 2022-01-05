#include "collisionResolver.h"

#include "engine/scene/components.h"
#include <iostream>




CollisionResolver::CollisionResolver(AABBColliderComponent& collider)
	: _collider(collider)
{
	// empty
}




void CollisionResolver::reset()
{
	_collisions.clear();
	_impulseForces.clear();
}


void CollisionResolver::addCollision(const AABBColliderComponent& otherCollider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse)
{
	_collisions.emplace_back(otherCollider, collisionNormal, relativeVelocity, impulse);
}


void CollisionResolver::processCollisions()
{
	if (_collider.rigidbody().type() == RigidbodyComponent::RigidbodyType::STATIC)
		return;

	for (const auto& collision : _collisions)
			_processCollision(collision);
}


void CollisionResolver::updateVelocity(Coords3f& velocity) const
{
	for (const auto& force : _impulseForces)
		velocity += force.value();
}




bool CollisionResolver::ignoreCollision(const RigidbodyComponent& rigidbody)
{
	return rigidbody.sleeping() || rigidbody.type() == RigidbodyComponent::RigidbodyType::STATIC;
}




void CollisionResolver::_processCollision(const Collision& collision)
{
	Coords3f impulseForce = collision.impulse() * _collider.rigidbody().invMass() * collision.collisionNormal();
	_collider.collisionResolver()->_impulseForces.emplace_back(Force::ForceType::LINEAR, impulseForce);
}