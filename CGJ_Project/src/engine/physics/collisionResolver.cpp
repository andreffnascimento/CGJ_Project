#include "collisionResolver.h"

#include "engine/scene/components.h"




CollisionResolver::CollisionResolver(AABBColliderComponent& collider)
	: _collider(collider)
{
	// empty
}




void CollisionResolver::reset()
{
	_collisions.clear();
	_impulses.clear();
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
	Coords3f finalImpulse = Coords3f();
	for (const auto& impulse : _impulses)
	{
		finalImpulse.x = std::signbit(finalImpulse.x) == std::signbit(impulse.x) ? std::max(finalImpulse.x, impulse.x) : finalImpulse.x + impulse.x;
		finalImpulse.y = std::signbit(finalImpulse.y) == std::signbit(impulse.y) ? std::max(finalImpulse.y, impulse.y) : finalImpulse.y + impulse.y;
		finalImpulse.z = std::signbit(finalImpulse.z) == std::signbit(impulse.z) ? std::max(finalImpulse.z, impulse.z) : finalImpulse.z + impulse.z;
	}

	velocity += finalImpulse;
}




bool CollisionResolver::ignoreCollision(const RigidbodyComponent& rigidbody)
{
	return rigidbody.sleeping() || rigidbody.type() == RigidbodyComponent::RigidbodyType::STATIC;
}




void CollisionResolver::_processCollision(const Collision& collision)
{
	Coords3f impulseForce = collision.impulse() * _collider.rigidbody().invMass() * collision.collisionNormal();
	_collider.collisionResolver()->_impulses.emplace_back(impulseForce);
}