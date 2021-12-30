#include "physicsEngine.h"

#include <cmath>

#include "engine/math/transform.h"
#include "engine/scene/scene.h"
#include "engine/utils/coords.h"




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


void PhysicsEngine::rotateBoundingBox(AABBColliderComponent& collider, const Coords3f& rotation)
{
	Coords3f p1 = Coords3f({ -collider._initialSize.x, -collider._initialSize.y,  collider._initialSize.z });
	Coords3f p2 = Coords3f({ -collider._initialSize.x,  collider._initialSize.y,  collider._initialSize.z });
	Coords3f p3 = Coords3f({  collider._initialSize.x, -collider._initialSize.y,  collider._initialSize.z });

	rotateVector(p1, rotation);
	rotateVector(p2, rotation);
	rotateVector(p3, rotation);

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




void PhysicsEngine::initialize(const Scene& scene) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent>& _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& iterator : _rigidbodyComponents)
	{
		EntityHandle entityId = iterator.first;
		RigidbodyComponent& rigidbody = iterator.second;

		Entity entity = scene.getEntityById(entityId);
		const TransformComponent& transform = entity.transform();

		rigidbody._position = transform.translation();
		rigidbody._rotation = transform.rotation();

		AABBColliderComponent* collider = entity.getComponentIfExists<AABBColliderComponent>();
		if (collider != nullptr)
		{
			PhysicsEngine::rotateBoundingBox(*collider, rigidbody._rotation);
			collider->_rigidbody = &rigidbody;
			collider->_transform = &transform;
		}
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
		if (!collider._rigidbody->_sleeping)
			PhysicsEngine::rotateBoundingBox(collider, collider._rigidbody->_rotation);
	}

	for (auto& iterator : _colliderComponents)
	{
		EntityHandle entityId = iterator.first;
		AABBColliderComponent& collider = iterator.second;
		if (!collider._rigidbody->_sleeping)
			_detectRigidbodyCollisions(scene, entityId, collider, ts);
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

#include <iostream>
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
	}
}




Coords3f PhysicsEngine::_calculateExpectedRotation(RigidbodyComponent& rigidbody, float ts) const
{
	Coords3f rotation = rigidbody._angularVelocity / ts;
	rigidbody._rotation += rotation;
	rigidbody._rotation.x = std::fmod(rigidbody._rotation.x, 360.0f);
	rigidbody._rotation.y = std::fmod(rigidbody._rotation.y, 360.0f);
	rigidbody._rotation.z = std::fmod(rigidbody._rotation.z, 360.0f);
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


void PhysicsEngine::_calculateExpectedVelocity(RigidbodyComponent& rigidbody, Coords3f& linearForce, float ts, const Coords3f& rotation) const
{
	linearForce += PhysicsEngine::calculateDragForce(rigidbody._velocity, rigidbody._drag, rigidbody._dragThreshold);
	linearForce *= (ts / rigidbody._mass);
	PhysicsEngine::rotateVector(linearForce, rotation);
	PhysicsEngine::rotateVector(rigidbody._velocity, rotation);
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