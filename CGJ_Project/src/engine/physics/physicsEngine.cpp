#include "physicsEngine.h"

#include <algorithm>

#include "engine/math/transform.h"
#include "engine/scene/scene.h"




void PhysicsEngine::rotateVector(Coords3f& vector, const Coords3f& rotation)
{
	if (rotation.x != 0.0f)
		PhysicsEngine::rotateVectorOnAxis(vector.z, vector.y, rotation.x);

	if (rotation.y != 0.0f)
		PhysicsEngine::rotateVectorOnAxis(vector.x, vector.z, rotation.y);

	if (rotation.z != 0.0f)
		PhysicsEngine::rotateVectorOnAxis(vector.y, vector.z, rotation.z);
}


void PhysicsEngine::rotateVectorOnAxis(float& coord1, float& coord2, float angle)
{
	float cosAngle = std::cos(toRadians(angle));
	float sinAngle = std::sin(toRadians(angle));
	float oldCoord1 = coord1;
	float oldCoord2 = coord2;
	coord1 = oldCoord1 * cosAngle + oldCoord2 * sinAngle;
	coord2 = oldCoord2 * cosAngle - oldCoord1 * sinAngle;
}


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
	float dragLength = -PhysicsEngine::AIR_DENSITY * drag * velocityLength * (velocityLength > dragThreshold ? velocityLength : PhysicsEngine::DRAG_SLOW_CONSTANT);
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
	std::unordered_map<EntityHandle, RigidbodyComponent>& _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& rigidbodyIterator : _rigidbodyComponents)
	{
		EntityHandle entityId = rigidbodyIterator.first;
		RigidbodyComponent& rigidbody = rigidbodyIterator.second;
		Entity entity = scene.getEntityById(entityId);
		const TransformComponent& transform = entity.transform();

		rigidbody._position = transform.translation();
		rigidbody._rotation = transform.rotation();
	}

	std::unordered_map<EntityHandle, AABBColliderComponent>& _colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (auto& colliderIterator : _colliderComponents)
	{
		EntityHandle entityId = colliderIterator.first;
		AABBColliderComponent& collider = colliderIterator.second;
		Entity entity = scene.getEntityById(entityId);

		collider._boundingBox = collider._initialSize;
		collider._rigidbody = entity.getComponentIfExists<RigidbodyComponent>();
		if (collider._rigidbody && !collider._fixedBoundingBox)
			PhysicsEngine::rotateBoundingBox(collider, collider._rigidbody->_rotation);
	}
}


void PhysicsEngine::simulate(const Scene& scene, float ts) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent>& _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	std::unordered_map<EntityHandle, AABBColliderComponent>& _colliderComponents = scene.getSceneComponents<AABBColliderComponent>();

	for (auto& iterator : _rigidbodyComponents)
	{
		RigidbodyComponent& rigidbody = iterator.second;
		if (!rigidbody._sleeping)
			_processRigidbodyMovement(scene, rigidbody, ts);
	}

	for (auto& iterator : _colliderComponents)
	{
		AABBColliderComponent& collider = iterator.second;
		if (!collider._rigidbody->_sleeping && !collider._fixedBoundingBox)
			PhysicsEngine::rotateBoundingBox(collider, collider._rigidbody->_rotation);
	}

	for (unsigned int i = 0; i < _collisionIterations; i++)
	{
		for (auto& iterator : _colliderComponents)
		{
			EntityHandle entityId = iterator.first;
			AABBColliderComponent& collider = iterator.second;
			if (!collider._rigidbody->_sleeping)
				_detectRigidbodyCollisions(scene, entityId, collider, ts);
		}
	}

	for (auto& iterator : _rigidbodyComponents)
	{
		EntityHandle entityId = iterator.first;
		RigidbodyComponent& rigidbody = iterator.second;
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


void PhysicsEngine::_detectRigidbodyCollisions(const Scene& scene, EntityHandle entityId, AABBColliderComponent& entityCollider, float ts) const
{
	std::unordered_map<EntityHandle, AABBColliderComponent>& _colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (auto& iterator : _colliderComponents)
	{
		EntityHandle otherId = iterator.first;
		AABBColliderComponent& otherCollider = iterator.second;
		if (entityId == otherId)
			continue;

		float entityMinX = entityCollider._rigidbody->_position.x - entityCollider._boundingBox.x;
		float entityMaxX = entityCollider._rigidbody->_position.x + entityCollider._boundingBox.x;
		float entityMinY = entityCollider._rigidbody->_position.y - entityCollider._boundingBox.y;
		float entityMaxY = entityCollider._rigidbody->_position.y + entityCollider._boundingBox.y;
		float entityMinZ = entityCollider._rigidbody->_position.z - entityCollider._boundingBox.z;
		float entityMaxZ = entityCollider._rigidbody->_position.z + entityCollider._boundingBox.z;

		float otherMinX = otherCollider._rigidbody->_position.x - otherCollider._boundingBox.x;
		float otherMaxX = otherCollider._rigidbody->_position.x + otherCollider._boundingBox.x;
		float otherMinY = otherCollider._rigidbody->_position.y - otherCollider._boundingBox.y;
		float otherMaxY = otherCollider._rigidbody->_position.y + otherCollider._boundingBox.y;
		float otherMinZ = otherCollider._rigidbody->_position.z - otherCollider._boundingBox.z;
		float otherMaxZ = otherCollider._rigidbody->_position.z + otherCollider._boundingBox.z;

		bool collision = (entityMinX <= otherMaxX && entityMaxX >= otherMinX) && 
						 (entityMinY <= otherMaxY && entityMaxY >= otherMinY) && 
						 (entityMinZ <= otherMaxZ && entityMaxZ >= otherMinZ);

		if (collision)
			_resolveCollision(entityCollider, otherCollider, ts);
	}
}




Coords3f PhysicsEngine::_calculateExpectedRotation(RigidbodyComponent& rigidbody, float ts) const
{
	Coords3f rotation = rigidbody._angularVelocity / ts;
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
	if (rigidbody._velocity.length2() > rigidbody._sleepThreshold || rigidbody._angularVelocity.length2() > rigidbody._sleepThreshold)
		return;

	rigidbody._velocity = Coords3f();
	rigidbody._angularVelocity = Coords3f();
	rigidbody._sleeping = true;
}




void PhysicsEngine::_resolveCollision(AABBColliderComponent& firstCollider, AABBColliderComponent& secondCollider, float ts) const
{
	RigidbodyComponent& firstRigidbody = *firstCollider._rigidbody;
	RigidbodyComponent& secondRigidbody = *secondCollider._rigidbody;

	Coords3f relativeVelocity = firstRigidbody._velocity - secondRigidbody._velocity;
	Coords3f collisionNormal = -firstRigidbody._velocity.normalized();

	float collisionSpeed = relativeVelocity.dot(collisionNormal);
	float restitutionCocoefficient = std::min(firstCollider._restitutionCocoefficient, secondCollider._restitutionCocoefficient);
	float firstInvMass = 1 / firstRigidbody._mass;
	float secondInvMass = 1 / secondRigidbody._mass;
	float impulseSpeed = -(1 + restitutionCocoefficient) * collisionSpeed;
	float impulse = impulseSpeed / (firstInvMass + secondInvMass);
	
	firstRigidbody._position -= firstRigidbody._velocity * ts;		// revert position update
	firstRigidbody._velocity += impulse * firstInvMass * collisionNormal;
	firstRigidbody._position += firstRigidbody._velocity * ts;
	firstRigidbody._sleeping = false;

	secondRigidbody._position -= secondRigidbody._velocity * ts;	// revert position update
	secondRigidbody._velocity -= impulse * secondInvMass * collisionNormal;
	secondRigidbody._position += secondRigidbody._velocity * ts;
	secondRigidbody._sleeping = false;
}