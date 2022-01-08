#ifndef __engine_physics_collisionResolver__
#define __engine_physics_collisionResolver__


#include <list>
#include <initializer_list>

#include "engine/physics/collision.h"

#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class CollisionResolver
{

private:
	enum class ComputeListType
	{
		WHITELIST = 1,
		BLACKLIST = 2
	};




protected:
	AABBColliderComponent& _collider; 

	std::list<Collision> _collisions = std::list<Collision>();
	std::list<Coords3f> _impulses = std::list<Coords3f>();


private:
	ComputeListType _computeListType = ComputeListType::WHITELIST;
	std::list<unsigned int> _computeIds = std::list<unsigned int>();




public:
	CollisionResolver() = delete;
	CollisionResolver(const CollisionResolver&) = default;
	CollisionResolver(AABBColliderComponent& collider);
	~CollisionResolver() = default;


public:
	inline const std::list<Collision>& collisions() const	{ return _collisions; }
	inline bool collided() const							{ return !_collisions.empty(); }
	inline bool updated() const								{ return _impulses.empty(); }


public:
	void reset();
	void addCollision(const AABBColliderComponent& otherCollider, const Coords3f& collisionNormal, const Coords3f& relativeVelocity, float impulse);
	void processCollisions();
	void updateVelocity(Coords3f& velocity) const;


public:
	bool isMoving() const;


public:
	virtual void init();
	virtual bool ignoreCollision(const AABBColliderComponent& otherCollider) const;




protected:
	void _setWhitelist(std::initializer_list<unsigned int> initializerList);
	void _setBlacklist(std::initializer_list<unsigned int> initializerList);


protected:
	virtual void _processCollision(const Collision& collision);

};


#endif // !__engine_physics_collisionResolver__