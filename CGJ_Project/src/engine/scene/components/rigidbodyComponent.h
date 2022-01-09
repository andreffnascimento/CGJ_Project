#ifndef __engine_scene_components_rigidbodyComponent__
#define __engine_scene_components_rigidbodyComponent__


#include <list>
#include <numeric>

#include "engine/math/quaternion.h"
#include "engine/physics/force.h"
#include "engine/utils/coords.h"




struct RigidbodyComponent
{
public:
	enum class RigidbodyType
	{
		STATIC,
		KINEMATIC,
		DYNAMIC
	};

private:
	RigidbodyType _type = RigidbodyComponent::RigidbodyType::DYNAMIC;
	float _mass = 1.0f;
	float _drag = 0.0f;
	float _angularDrag = 0.0f;
	float _dragThreshold = 1.0f;
	bool _usesGravity = false;

	const TransformComponent* transform = nullptr;		// pointer to the entity transform for efficiency reasons
	Coords3f _position = Coords3f();
	Quaternion _rotation = Coords3f();
	Coords3f _velocity = Coords3f();
	Coords3f _angularVelocity = Coords3f();

	float _sleepThreshold = 0.1f;			// smallest velocity that makes the object go to sleep
	bool _sleeping = true;					// doesn't process the objects if no changes occur

	std::list<Force> _forces = std::list<Force>();

public:
	RigidbodyComponent() = delete;
	RigidbodyComponent(const RigidbodyComponent&) = default;
	RigidbodyComponent(RigidbodyComponent::RigidbodyType type);
	RigidbodyComponent(RigidbodyComponent::RigidbodyType type, float mass, float drag, float angularDrag);
	~RigidbodyComponent() = default;

	inline const RigidbodyComponent::RigidbodyType type() const { return _type; }
	inline float mass() const						{ return  _mass == 0.0f ? std::numeric_limits<float>::max() : 1.0f / _mass; }
	inline float invMass() const					{ return _mass; }
	inline float drag() const						{ return _drag; }
	inline float angularDrag() const				{ return _angularDrag; }
	inline float dragThreshold() const				{ return _dragThreshold; }
	inline bool usesGravity() const					{ return _usesGravity; }
	inline const Coords3f& position() const			{ return _position; }
	inline const Quaternion& rotation() const		{ return _rotation; }
	inline const Coords3f& velocity() const			{ return _velocity; }
	inline const Coords3f& angularVelocity() const	{ return _angularVelocity; }
	inline bool sleeping() const					{ return _sleeping; }

	inline void setDragThreshold(float dragThreshold)	{ _dragThreshold = dragThreshold; }
	inline void setUsesGravity(bool usesGravity)		{ _usesGravity = usesGravity; }
	inline void setSleepThreshold(float sleepThreshold) { _sleepThreshold = sleepThreshold; }

	inline void addRelativeForce(const Coords3f& value) { addRelativeForce(Force(Force::ForceType::LINEAR, value)); }
	inline void addAbsoluteForce(const Coords3f& value) { addAbsoluteForce(Force(Force::ForceType::LINEAR, value)); }
	inline void addAngularForce(const Coords3f& value)	{ addAbsoluteForce(Force(Force::ForceType::ANGULAR, value)); }

	void setVelocity(const Coords3f& velocity);
	void setAngularVelocity(const Coords3f& angularVelocity);
	void addAbsoluteForce(const Force& force);
	void addRelativeForce(const Force& force);

public:
	friend class Transform;
	friend class PhysicsEngine;
};


#endif // !__engine_scene_components_rigidbodyComponent__