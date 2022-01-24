#ifndef __engine_physics_physicsEngine__
#define __engine_physics_physicsEngine__


#include <unordered_map>

#include "engine/physics/force.h"

#include "engine/scene/entity.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"


class Scene;




class PhysicsEngine
{

public:
	static constexpr float GRAVITY = 2.0f * 9.8f;
	static constexpr float AIR_DENSITY = 1.225f;
	static constexpr float DRAG_SLOW_CONSTANT = 10.0f;

	static constexpr bool SHOW_COLLIDERS = false;
	static constexpr unsigned int COLLISION_ITERATIONS = 4;
	static constexpr float PHYSICS_SIMULATION_MIN_INTERVAL = 0.02f;
	static constexpr float PHYSICS_SIMULATION_MAX_INTERVAL = 0.13f;




private:
	std::unordered_map<EntityHandle, AABBColliderComponent*> _activeColliders = std::unordered_map<EntityHandle, AABBColliderComponent*>();




public:
	static void rotateBoundingBox(AABBColliderComponent& collider, const Quaternion& rotation);
	static Coords3f calculateDragForce(const Coords3f& velocity, float drag, float dragThreshold);




public:
	PhysicsEngine() = default;
	PhysicsEngine(const PhysicsEngine&) = default;
	~PhysicsEngine() = default;


public:
	inline constexpr bool showColliders() const { return PhysicsEngine::SHOW_COLLIDERS; }


public:
	void initialize(const Scene& scene) const;
	void simulate(const Scene& scene, float ts);
	void syncTransforms(const Scene& scene) const;
	void updateTransforms(const Scene& scene);




private:
	void _initializeRigidbodies(const Scene& scene) const;
	void _initializeColliders(const Scene& scene) const;
	void _syncRigidbodyWithTransform(const Entity& entity, RigidbodyComponent& rigidbody) const;


private:
	void _simulateRigidbodyMovement(const Scene& scene, float ts);
	void _processRigidbodyMovement(const Entity& entity, RigidbodyComponent& rigidbody, float ts);
	void _combineForces(RigidbodyComponent& rigidbody, Coords3f& linearForce, Coords3f& angularForce) const;
	void _calculateFinalAngularForce(RigidbodyComponent& rigidbody, Coords3f& angularForce) const;
	void _calculateFinalLinearForce(RigidbodyComponent& rigidbody, Coords3f& linearForce) const;
	void _calculateExpectedAngularVelocity(RigidbodyComponent& rigidbody, Coords3f& angularForce, float ts) const;
	void _calculateExpectedLinearVelocity(RigidbodyComponent& rigidbody, Coords3f& linearForce, float ts) const;
	void _processSleepThreshold(const Entity& entity, RigidbodyComponent& rigidbody);


private:
	void _simulateCollisions(const Scene& scene, float ts);
	void _resetCollider(const Scene& scene) const;
	void _resolveCollisions(const Scene& scene, float ts);
	void _checkCollision(AABBColliderComponent& entityCollider, AABBColliderComponent& otherCollider, float ts) const;
	void _resolveCollision(AABBColliderComponent& entityCollider, AABBColliderComponent& otherCollider, const bool* contained, float ts) const;
	void _addActiveCollider(const Entity& entity);


private:
	void _simulateParticles(const Scene& scene, float ts) const;
	void _simulateParticleGenerator(ParticleGeneratorComponent& particleGenerator, float ts) const;

};


#endif // !__engine_physics_physicsEngine__