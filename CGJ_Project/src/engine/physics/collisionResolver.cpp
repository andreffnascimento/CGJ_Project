#include "collisionResolver.h"

#include "engine/scene/components.h"




CollisionResolver::CollisionResolver(AABBColliderComponent& collider)
	: _collider(collider)
{
	// empty
}




void CollisionResolver::addCollision(const AABBColliderComponent& otherCollider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse)
{
	if (_collider.rigidbody().type() == RigidbodyComponent::RigidbodyType::STATIC)
		return;

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
		finalImpulse.x = std::signbit(finalImpulse.x) == std::signbit(impulse.x) ? std::max(finalImpulse.x, impulse.x) : finalImpulse.x + impulse.x;
		finalImpulse.y = std::signbit(finalImpulse.y) == std::signbit(impulse.y) ? std::max(finalImpulse.y, impulse.y) : finalImpulse.y + impulse.y;
		finalImpulse.z = std::signbit(finalImpulse.z) == std::signbit(impulse.z) ? std::max(finalImpulse.z, impulse.z) : finalImpulse.z + impulse.z;
	}

	velocity += finalImpulse;
	_impulses.clear();
}




bool CollisionResolver::isMoving() const
{
	return !_collider.rigidbody().sleeping() && _collider.rigidbody().type() != RigidbodyComponent::RigidbodyType::STATIC;
}




void CollisionResolver::init()
{
	// empty
}


bool CollisionResolver::ignoreCollision(const AABBColliderComponent& otherCollider) const
{
	bool whitelist = _computeListType == CollisionResolver::ComputeListType::WHITELIST;
	return whitelist != (bool)(_computeIdsFlag & (unsigned long long)otherCollider.id());
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
	Coords3f impulseForce = collision.impulse() * _collider.rigidbody().invMass() * collision.collisionNormal();
	_collider.collisionResolver()->_impulses.emplace_back(impulseForce);
}