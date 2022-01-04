#include "physicsEngine.h"

#include <algorithm>
#include <memory>

#include "engine/physics/collisionResolver.h"

#include "engine/math/transform.h"
#include "engine/scene/scene.h"




void PhysicsEngine::rotateBoundingBox(AABBColliderComponent& collider, const Quaternion& rotation)
{
	Coords3f p1 = Coords3f({ -collider._initialSize.x, -collider._initialSize.y,  collider._initialSize.z });
	Coords3f p2 = Coords3f({ -collider._initialSize.x,  collider._initialSize.y,  collider._initialSize.z });
	Coords3f p3 = Coords3f({  collider._initialSize.x, -collider._initialSize.y,  collider._initialSize.z });

	rotation.rotatePoint(p1);
	rotation.rotatePoint(p2);
	rotation.rotatePoint(p3);

	float xMax = std::max(std::abs(p1.x), std::max(std::abs(p2.x), std::abs(p3.x)));
	float yMax = std::max(std::abs(p1.y), std::max(std::abs(p2.y), std::abs(p3.y)));
	float zMax = std::max(std::abs(p1.z), std::max(std::abs(p2.z), std::abs(p3.z)));

	collider._boundingBox = Coords3f({ xMax, yMax, zMax });
}


Coords3f PhysicsEngine::calculateDragForce(const Coords3f& velocity, float drag, float dragThreshold)
{
	float velocityLength = velocity.length();
	float dragLength = -PhysicsEngine::AIR_DENSITY * drag * velocityLength * (velocityLength * velocityLength > dragThreshold ? velocityLength : PhysicsEngine::DRAG_SLOW_CONSTANT);
	Coords3f dragForce = velocity.normalized();
	dragForce *= dragLength;
	return dragForce;
}




PhysicsEngine::PhysicsEngine(unsigned int collisionIterations)
	:_collisionIterations(collisionIterations)
{
	// empty
}




void PhysicsEngine::initialize(const Scene& scene) const
{
	_initializeRigidbodies(scene);
	_initializeColliders(scene);
}


void PhysicsEngine::simulate(const Scene& scene, float ts) const
{
	_simulateRigidbodyMovement(scene, ts);
	_simulateCollisions(scene, ts);
	_updateRigidbodyTransform(scene);
}




void PhysicsEngine::_initializeRigidbodies(const Scene& scene) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent>& _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& rigidbodyIterator : _rigidbodyComponents)
	{
		EntityHandle entityId = rigidbodyIterator.first;
		RigidbodyComponent& rigidbody = rigidbodyIterator.second;
		const TransformComponent& transform = scene.getEntityById(entityId).transform();

		rigidbody._position = transform.translation();
		rigidbody._rotation = transform.rotation();
	}
}


void PhysicsEngine::_initializeColliders(const Scene& scene) const
{
	std::unordered_map<EntityHandle, AABBColliderComponent>& _colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (auto& colliderIterator : _colliderComponents)
	{
		AABBColliderComponent& collider = colliderIterator.second;
		collider._boundingBox = collider._initialSize;

		if (collider._collisionResolver == nullptr)
			collider._collisionResolver = new CollisionResolver();

		if (!collider._fixedBoundingBox)
			PhysicsEngine::rotateBoundingBox(collider, collider._rigidbody->_rotation);
	}
}




void PhysicsEngine::_simulateRigidbodyMovement(const Scene& scene, float ts) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent>& rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& rigidbodyIterator : rigidbodyComponents)
	{
		RigidbodyComponent& rigidbody = rigidbodyIterator.second;
		if (!rigidbody._sleeping)
			_processRigidbodyMovement(scene, rigidbody, ts);
	}
}


void PhysicsEngine::_simulateCollisions(const Scene& scene, float ts) const
{
	std::unordered_map<EntityHandle, AABBColliderComponent>& colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (unsigned int i = 0; i < _collisionIterations; i++)
	{
		_resetCollider(scene);
		for (auto& entityColliderIterator = colliderComponents.begin(); entityColliderIterator != colliderComponents.end(); entityColliderIterator++)
		{
			AABBColliderComponent& entityCollider = entityColliderIterator->second;
			for (auto& otherColliderIterator = std::next(entityColliderIterator); otherColliderIterator != colliderComponents.end(); otherColliderIterator++)
			{
				AABBColliderComponent& otherCollider = otherColliderIterator->second;
				_checkCollision(entityCollider, otherCollider, ts);
			}
		}
		_resolveCollisions(scene, ts);
	}
}


void PhysicsEngine::_resetCollider(const Scene& scene) const
{
	std::unordered_map<EntityHandle, AABBColliderComponent>& colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (auto& colliderIterator : colliderComponents)
	{
		AABBColliderComponent& collider = colliderIterator.second;
		collider._collisionResolver->reset();
		if (!collider._rigidbody->_sleeping && !collider._fixedBoundingBox)
			PhysicsEngine::rotateBoundingBox(collider, collider._rigidbody->_rotation);
	}
}


void PhysicsEngine::_resolveCollisions(const Scene& scene, float ts) const
{
	std::unordered_map<EntityHandle, AABBColliderComponent>& colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (auto& colliderIterator : colliderComponents)
	{
		AABBColliderComponent& collider = colliderIterator.second;
		if (!collider._collisionResolver->collided())
			continue;

		collider._collisionResolver->processCollisions(collider);
		if (!collider._collisionResolver->updated())
		{
			RigidbodyComponent& rigidbody = *collider._rigidbody;
			rigidbody._position -= rigidbody._velocity * ts;		// revert position update
			collider._collisionResolver->updateVelocity(rigidbody._velocity);
			rigidbody._position += rigidbody._velocity * ts;
			rigidbody._sleeping = false;
		}
	}
}


void PhysicsEngine::_updateRigidbodyTransform(const Scene& scene) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent>& rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& rigidbodyIterator : rigidbodyComponents)
	{
		EntityHandle entityId = rigidbodyIterator.first;
		RigidbodyComponent& rigidbody = rigidbodyIterator.second;
		if (!rigidbody._sleeping)
		{
			Entity entity = scene.getEntityById(entityId);
			Transform::translateTo(entity, rigidbody._position);
			Transform::rotateTo(entity, rigidbody._rotation);
		}
	}
}




void PhysicsEngine::_processRigidbodyMovement(const Scene& scene, RigidbodyComponent& rigidbody, float ts) const
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

	rigidbody._position += rigidbody._velocity * ts;
	_processSleepThreshold(rigidbody);
}

Coords3f PhysicsEngine::_calculateExpectedRotation(RigidbodyComponent& rigidbody, float ts) const
{
	Coords3f rotation = rigidbody._angularVelocity * ts;
	rigidbody._rotation.rotate(rotation);
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
	angularForce += PhysicsEngine::calculateDragForce(rigidbody._angularVelocity, rigidbody._angularDrag, rigidbody._dragThreshold);
	angularForce *= (ts / rigidbody._mass);
	rigidbody._angularVelocity += angularForce;
}


void PhysicsEngine::_calculateExpectedVelocity(RigidbodyComponent& rigidbody, Coords3f& linearForce, float ts, const Quaternion& rotation) const
{
	linearForce += PhysicsEngine::calculateDragForce(rigidbody._velocity, rigidbody._drag, rigidbody._dragThreshold);
	linearForce *= (ts / rigidbody._mass);

	rotation.rotatePoint(linearForce);
	rotation.rotatePoint(rigidbody._velocity);

	rigidbody._velocity += linearForce;
}


void PhysicsEngine::_processSleepThreshold(RigidbodyComponent& rigidbody) const
{
	if (rigidbody._velocity.length() > rigidbody._sleepThreshold || rigidbody._angularVelocity.length() > rigidbody._sleepThreshold)
		return;

	rigidbody._velocity = Coords3f();
	rigidbody._angularVelocity = Coords3f();
	rigidbody._sleeping = true;
}



void PhysicsEngine::_checkCollision(AABBColliderComponent& entityCollider, AABBColliderComponent& otherCollider, float ts) const
{
	RigidbodyComponent& entityRigidbody = *entityCollider._rigidbody;
	RigidbodyComponent& otherRigidbody = *otherCollider._rigidbody;

	bool ignoreEntityCollider = entityCollider._collisionResolver->ignoreCollision(entityRigidbody);
	bool ignoreOtherCollider = otherCollider._collisionResolver->ignoreCollision(otherRigidbody);

	if (ignoreEntityCollider && ignoreOtherCollider)
		return;

	float entityMinX = entityRigidbody._position.x - entityCollider._boundingBox.x;
	float entityMaxX = entityRigidbody._position.x + entityCollider._boundingBox.x;
	float entityMinY = entityRigidbody._position.y - entityCollider._boundingBox.y;
	float entityMaxY = entityRigidbody._position.y + entityCollider._boundingBox.y;
	float entityMinZ = entityRigidbody._position.z - entityCollider._boundingBox.z;
	float entityMaxZ = entityRigidbody._position.z + entityCollider._boundingBox.z;

	float otherMinX = otherRigidbody._position.x - otherCollider._boundingBox.x;
	float otherMaxX = otherRigidbody._position.x + otherCollider._boundingBox.x;
	float otherMinY = otherRigidbody._position.y - otherCollider._boundingBox.y;
	float otherMaxY = otherRigidbody._position.y + otherCollider._boundingBox.y;
	float otherMinZ = otherRigidbody._position.z - otherCollider._boundingBox.z;
	float otherMaxZ = otherRigidbody._position.z + otherCollider._boundingBox.z;

	bool collision = (entityMinX <= otherMaxX && entityMaxX >= otherMinX) &&
		(entityMinY <= otherMaxY && entityMaxY >= otherMinY) &&
		(entityMinZ <= otherMaxZ && entityMaxZ >= otherMinZ);

	if (collision)
	{
		AABBColliderComponent& fixedEntityCollider = ignoreEntityCollider ? otherCollider : entityCollider;
		AABBColliderComponent& fixedOtherCollider = ignoreEntityCollider ? entityCollider : otherCollider;
		_resolveCollision(fixedEntityCollider, fixedOtherCollider, ts);
	}
}


void PhysicsEngine::_resolveCollision(AABBColliderComponent& entityCollider, AABBColliderComponent& otherCollider, float ts) const
{
	RigidbodyComponent& entityRigidbody = *entityCollider._rigidbody;
	RigidbodyComponent& otherRigidbody = *otherCollider._rigidbody;
	
	Coords3f collisionNormal = entityRigidbody._velocity.normalized();
	Coords3f relativeVelocity = entityRigidbody._velocity - otherRigidbody._velocity;

	Coords3f testPosition = entityRigidbody._position + entityRigidbody._velocity * ts;
	if (otherRigidbody._position.distance(testPosition) > otherRigidbody._position.distance(entityRigidbody._position))
		return;

	float entityInvMass = 1.0f / entityRigidbody._mass;
	float otherInvMass = 1.0f / otherRigidbody._mass;
	float collisionSpeed = relativeVelocity.dot(collisionNormal);
	float restitutionCocoefficient = std::min(entityCollider._restitutionCocoefficient, otherCollider._restitutionCocoefficient);
	float impulseSpeed = -(1 + restitutionCocoefficient) * collisionSpeed;
	float impulse = impulseSpeed / (entityInvMass + otherInvMass);

	entityCollider._collisionResolver->addCollision(otherCollider, collisionNormal, relativeVelocity,  impulse * entityInvMass);
	otherCollider._collisionResolver->addCollision(entityCollider, collisionNormal, relativeVelocity, -impulse * otherInvMass);
}