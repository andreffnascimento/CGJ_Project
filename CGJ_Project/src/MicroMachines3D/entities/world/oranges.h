#ifndef __mm3d_entities_world_oranges__
#define __mm3d_entities_world_oranges__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/oranges/orangesManagerScript.h"
#include "MicroMachines3D/scripts/collisionResolver/orangeCollisionResolver.h"




class Oranges : public SceneEntity
{

public:
	Oranges(Scene* scene)
		: SceneEntity(scene->createEntity("Oranges"))
	{
		GroupComponent& group = addComponent<GroupComponent>();
		MeshComponent mesh = MeshComponent(createSphere(ORANGE_RADIUS, ORANGE_DIVISIONS), ORANGE_MATERIAL);

		for (int i = 0; i < N_ORANGES; i++)
			_createOrange(scene, group, mesh, i);

		addComponent<ScriptComponent>(std::make_shared<OrangesManagerScript>(scene));
	}


private:
	void _createOrange(Scene* scene, GroupComponent& group, const MeshComponent& mesh, unsigned int id)
	{
		Entity orange = group.addNewEntity(scene, *this, "orange_" + std::to_string(id));
		orange.addComponent<MeshComponent>(mesh, orange);

		RigidbodyComponent& rigidbody = orange.addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::KINEMATIC);
		AABBColliderComponent& collider = orange.addComponent<AABBColliderComponent>(ColliderIds::ORANGES, rigidbody, ORANGE_SIZE);
		collider.setCollisionResolver(new OrangeCollisionResolver(collider));
		collider.setFixedBoundingBox(true);
	}

};

#endif // !__mm3d_entities_world_oranges__