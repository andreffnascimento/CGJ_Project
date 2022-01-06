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
	float dragLength = -PhysicsEngine::AIR_DENSITY * drag * velocityLength * (velocityLength > dragThreshold ? velocityLength : PhysicsEngine::DRAG_SLOW_CONSTANT);
	Coords3f dragForce = velocity.normalized();
	dragForce *= dragLength;
	return dragForce;
}




void PhysicsEngine::initialize(const Scene& scene) const
{
	_initializeRigidbodies(scene);
	_initializeColliders(scene);
}


void PhysicsEngine::simulate(const Scene& scene, float ts)
{
	_simulateRigidbodyMovement(scene, ts);
	_simulateCollisions(scene, ts);
}


void PhysicsEngine::updateTransforms(const Scene& scene)
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




void PhysicsEngine::_initializeRigidbodies(const Scene& scene) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent>& _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& rigidbodyIterator : _rigidbodyComponents)
	{
		EntityHandle entityId = rigidbodyIterator.first;
		RigidbodyComponent& rigidbody = rigidbodyIterator.second;
		Entity entity = scene.getEntityById(entityId);

		Coords3f translation;
		Quaternion rotation;
		Coords3f scale;
		Transform::decomposeTransformMatrix(entity, translation, rotation, scale);

		// HACK: this may very easily break when dealing with group movements
		rigidbody._position = translation;
		rigidbody._rotation = rotation;
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
			collider._collisionResolver = new CollisionResolver(collider);

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
		if (!rigidbody._sleeping || rigidbody._usesGravity)
			_processRigidbodyMovement(scene, rigidbody, ts);
	}
}


void PhysicsEngine::_simulateCollisions(const Scene& scene, float ts) const
{
	std::unordered_map<EntityHandle, AABBColliderComponent>& colliderComponents = scene.getSceneComponents<AABBColliderComponent>();
	for (unsigned int i = 0; i < PhysicsEngine::COLLISION_ITERATIONS; i++)
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

		collider._collisionResolver->processCollisions();
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




void PhysicsEngine::_processRigidbodyMovement(const Scene& scene, RigidbodyComponent& rigidbody, float ts) const
{
	Coords3f linearForce = Coords3f();
	Coords3f angularForce = Coords3f();

	rigidbody._sleeping = false;
	if (rigidbody._type == RigidbodyComponent::RigidbodyType::DYNAMIC)
	{
		_combineForces(rigidbody, linearForce, angularForce);
		_calculateFinalAngularForce(rigidbody, angularForce);
		_calculateFinalLinearForce(rigidbody, linearForce);
	}

	Coords3f rotation = _calculateExpectedRotation(rigidbody, ts);
	_calculateExpectedPosition(rigidbody, linearForce, ts);
	_calculateExpectedAngularVelocity(rigidbody, angularForce, ts);
	_calculateExpectedLinearVelocity(rigidbody, linearForce, Quaternion(rotation), ts);
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
}


void PhysicsEngine::_calculateFinalAngularForce(RigidbodyComponent& rigidbody, Coords3f& angularForce) const
{
	angularForce += PhysicsEngine::calculateDragForce(rigidbody._angularVelocity, rigidbody._angularDrag, rigidbody._dragThreshold);
	angularForce *= rigidbody._mass;
}


void PhysicsEngine::_calculateFinalLinearForce(RigidbodyComponent& rigidbody, Coords3f& linearForce) const
{
	linearForce += PhysicsEngine::calculateDragForce(rigidbody._velocity, rigidbody._drag, rigidbody._dragThreshold);
	linearForce *= rigidbody._mass;

	if (rigidbody._usesGravity)
		linearForce.y -= PhysicsEngine::GRAVITY;
}


void PhysicsEngine::_calculateExpectedPosition(RigidbodyComponent& rigidbody, const Coords3f& linearForce, float ts) const
{
	float time2 = std::pow(ts, 2.0f) / 2.0f;
	rigidbody._position.x += rigidbody._velocity.x * ts + linearForce.x * time2;
	rigidbody._position.y += rigidbody._velocity.y * ts + linearForce.y * time2;
	rigidbody._position.z += rigidbody._velocity.z * ts + linearForce.z * time2;
}


void PhysicsEngine::_calculateExpectedAngularVelocity(RigidbodyComponent& rigidbody, Coords3f& angularForce, float ts) const
{
	angularForce *= ts;
	rigidbody._angularVelocity += angularForce;
}


void PhysicsEngine::_calculateExpectedLinearVelocity(RigidbodyComponent& rigidbody, Coords3f& linearForce, const Quaternion& rotation, float ts) const
{
	rotation.rotatePoint(linearForce);
	rotation.rotatePoint(rigidbody._velocity);
	linearForce *= ts;
	rigidbody._velocity += linearForce;
}


void PhysicsEngine::_processSleepThreshold(RigidbodyComponent& rigidbody) const
{
	rigidbody._forces.clear();
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

	float entityCoords[6] = {};
	float otherCoords[6] = {};

	AABBColliderComponent* entityColliderPtr = &entityCollider;
	AABBColliderComponent* otherColliderPtr = &otherCollider;
	float* entityPtr = entityCoords;
	float* otherPtr = otherCoords;

	if (ignoreEntityCollider)
	{
		entityColliderPtr = &otherCollider;
		otherColliderPtr = &entityCollider;
		entityPtr = otherCoords;
		otherPtr = entityCoords;
	}

	entityCoords[0] = entityRigidbody._position.x - entityCollider._boundingBox.x;
	entityCoords[1] = entityRigidbody._position.x + entityCollider._boundingBox.x;
	otherCoords[0] = otherRigidbody._position.x - otherCollider._boundingBox.x;
	otherCoords[1] = otherRigidbody._position.x + otherCollider._boundingBox.x;
	if (entityCoords[0] > otherCoords[1] || otherCoords[0] > entityCoords[1])
		return;

	entityCoords[2] = entityRigidbody._position.y - entityCollider._boundingBox.y;
	entityCoords[3] = entityRigidbody._position.y + entityCollider._boundingBox.y;
	otherCoords[2] = otherRigidbody._position.y - otherCollider._boundingBox.y;
	otherCoords[3] = otherRigidbody._position.y + otherCollider._boundingBox.y;
	if (entityCoords[2] > otherCoords[3] || otherCoords[2] > entityCoords[3])
		return;

	entityCoords[4] = entityRigidbody._position.z - entityCollider._boundingBox.z;
	entityCoords[5] = entityRigidbody._position.z + entityCollider._boundingBox.z;
	otherCoords[4] = otherRigidbody._position.z - otherCollider._boundingBox.z;
	otherCoords[5] = otherRigidbody._position.z + otherCollider._boundingBox.z;
	if (entityCoords[4] > otherCoords[5] || otherCoords[4] > entityCoords[5])
		return;

	bool contained[3] = {};
	contained[0] = otherPtr[0] < entityPtr[0] && entityPtr[1] < otherPtr[1];
	contained[1] = otherPtr[2] < entityPtr[2] && entityPtr[3] < otherPtr[3];
	contained[2] = otherPtr[4] < entityPtr[4] && entityPtr[5] < otherPtr[5];
	_resolveCollision(*entityColliderPtr, *otherColliderPtr, contained, ts);
}


void PhysicsEngine::_resolveCollision(AABBColliderComponent& entityCollider, AABBColliderComponent& otherCollider, const bool* contained, float ts) const
{
	RigidbodyComponent& entityRigidbody = *entityCollider._rigidbody;
	RigidbodyComponent& otherRigidbody = *otherCollider._rigidbody;
	
	Coords3f relativeVelocity = entityRigidbody._velocity - otherRigidbody._velocity;
	Coords3f collisionNormal = entityRigidbody._velocity;
	if (contained[0] || std::signbit(otherRigidbody._position.x - entityRigidbody._position.x) != std::signbit(entityRigidbody._velocity.x))
		collisionNormal.x = 0.0f;
	if (contained[1] || std::signbit(otherRigidbody._position.y - entityRigidbody._position.y) != std::signbit(entityRigidbody._velocity.y))
		collisionNormal.y = 0.0f;
	if (contained[2] || std::signbit(otherRigidbody._position.z - entityRigidbody._position.z) != std::signbit(entityRigidbody._velocity.z))
		collisionNormal.z = 0.0f;
	collisionNormal.normalize();

	float collisionSpeed = collisionNormal.dot(relativeVelocity);
	if (collisionSpeed < 0)
		return;

	float restitutionCocoefficient = std::min(entityCollider._restitutionCocoefficient, otherCollider._restitutionCocoefficient);
	float impulseSpeed = -(1 + restitutionCocoefficient) * collisionSpeed;
	float impulse = impulseSpeed / (entityRigidbody._mass + otherRigidbody._mass);

	entityCollider._collisionResolver->addCollision(otherCollider,  collisionNormal, relativeVelocity, impulse);
	otherCollider._collisionResolver->addCollision(entityCollider, -collisionNormal, relativeVelocity, impulse);
}