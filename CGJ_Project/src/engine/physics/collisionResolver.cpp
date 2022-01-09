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
	_initCollisionProcessing();
	for (const auto& collision : _collisions)
		_processCollision(collision);

	_collisions.clear();
	_terminateCollisionProcessing();
}


void CollisionResolver::updateVelocity(Coords3f& velocity)
{
	_initVelocityProcessing(velocity);
	for (const auto& impulse : _impulses)
		_processVelocity(velocity, impulse);

	_impulses.clear();
	_terminateVelocityProcessing(velocity);
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




void CollisionResolver::_initCollisionProcessing()
{
	// empty
}


void CollisionResolver::_processCollision(const Collision& collision)
{
	Coords3f impulseForce = collision.impulse() * _collider.rigidbody().invMass() * collision.collisionNormal();
	_collider.collisionResolver()->_impulses.emplace_back(impulseForce);
}


void CollisionResolver::_terminateCollisionProcessing()
{
	// empty
}


void CollisionResolver::_initVelocityProcessing(Coords3f& velocity)
{
	// empty
}


void CollisionResolver::_processVelocity(Coords3f& velocity, const Coords3f& impulse)
{
	velocity += impulse;
}


void CollisionResolver::_terminateVelocityProcessing(Coords3f& velocity)
{
	// empty
}