#include "physicsEngine.h"

#include <unordered_map>

#include "engine/scene/scene.h"
#include "engine/scene/components.h"




void PhysicsEngine::run(const Scene& scene) const
{
	std::unordered_map<EntityHandle, RigidbodyComponent> _rigidbodyComponents = scene.getSceneComponents<RigidbodyComponent>();
	for (auto& iterator : _rigidbodyComponents)
	{
		EntityHandle entityId = iterator.first;
		RigidbodyComponent& rigidbody = iterator.second;
	}
}