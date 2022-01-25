#include "collisionResolver.h"

#include "engine/scene/components.h"




CollisionResolver::CollisionResolver(AABBColliderComponent& collider)
	: _collider(collider)
{
	// empty
}



#include <iostream>
void CollisionResolver::addCollision(const AABBColliderComponent& otherCollider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse)
{
	if (_collider.rigidbody()->type() != RigidbodyComponent::RigidbodyType::STATIC)
		_collisions.emplace_back(otherCollider, collisionNormal, relativeVelocity, impulse);
}


void CollisionResolver::processCollisions()
{
	for (const auto& collision : _collisions)
		_processCollision(collision);

	_collisions.clear();
}


void CollisionResolver::updateVelocity(Coords3f& velocity)
{
	Coords3f finalImpulse = Coords3f();
	for (const auto& impulse : _impulses)
	{
		finalImpulse.x = std::abs(finalImpulse.x) > std::abs(impulse.x) ? finalImpulse.x + impulse.x * 0.2f : impulse.x + finalImpulse.x * 0.2f;
		finalImpulse.y = std::abs(finalImpulse.y) > std::abs(impulse.y) ? finalImpulse.y + impulse.y * 0.2f : impulse.y + finalImpulse.y * 0.2f;
		finalImpulse.z = std::abs(finalImpulse.z) > std::abs(impulse.z) ? finalImpulse.z + impulse.z * 0.2f : impulse.z + finalImpulse.z * 0.2f;
	}

	_impulses.clear();
	velocity += finalImpulse;
}




bool CollisionResolver::isMoving() const
{
	return !_collider.rigidbody()->sleeping() && _collider.rigidbody()->type() != RigidbodyComponent::RigidbodyType::STATIC;
}


bool CollisionResolver::checkCollisionId(unsigned long long colliderId) const
{
	bool whitelist = _computeListType == CollisionResolver::ComputeListType::WHITELIST;
	return whitelist == (bool)(_computeIdsFlag & colliderId);
}




void CollisionResolver::init()
{
	// empty
}




void CollisionResolver::_setWhitelist(unsigned long long computeIdsFlag)
{
	_computeListType = CollisionResolver::ComputeListType::WHITELIST;
	_computeIdsFlag = computeIdsFlag;
}

void CollisionResolver::_setBlacklist(unsigned long long computeIdsFlag)
{
	_computeListType = CollisionResolver::ComputeListType::BLACKLIST;
	_computeIdsFlag = computeIdsFlag;
}




void CollisionResolver::_processCollision(const Collision& collision)
{
	Coords3f impulseForce = collision.impulse() * _collider.rigidbody()->invMass() * collision.collisionNormal();
	_collider.collisionResolver()->_impulses.emplace_back(impulseForce);
}