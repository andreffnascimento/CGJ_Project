#ifndef __mm3d_entities_world_orange__
#define __mm3d_entities_world_orange__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/oranges/orangeMovementScript.h"




class Orange : public SceneEntity
{

public:
	Orange(Scene* scene, std::string tag, Coords3f position, Coords3f movementDirection)
		: SceneEntity(scene->createEntity(tag))
	{
		RigidbodyComponent& rigidbody = addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::KINEMATIC);
		AABBColliderComponent& collider = addComponent<AABBColliderComponent>(rigidbody, ORANGE_SIZE);

		MeshComponent mesh = MeshComponent(createSphere(ORANGE_RADIUS, ORANGE_DIVISIONS), ORANGE_MATERIAL);

		// TODO texture 

		ScriptComponent& script = addComponent<ScriptComponent>();
		script.addScriptInstance(std::make_shared<OrangeMovementScript>(scene, tag, movementDirection));

		Transform::translate(*this, position);
	}
};

#endif // !__mm3d_entities_world_orange__
