#ifndef __mm3d_entities_world_rearViewMirror__
#define __mm3d_entities_world_rearViewMirror__


#include "MicroMachines3D/common/include.h"



class RearViewMirror : public SceneEntity
{

public:
	RearViewMirror(Scene* scene)
		: SceneEntity(scene->createEntity("RearViewMirror"))
	{
		//GroupComponent& group = addComponent<GroupComponent>();

		//RigidbodyComponent& rigidbody = addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::DYNAMIC, 100.0f, 10.0f, 30.0f);
		MeshComponent mirrorMesh =  MeshComponent(createCube(), TABLE_MATERIAL);
		
		addComponent<MeshComponent>(mirrorMesh);

		//mirrorMesh.setIsMirror(true);

		Transform::scale(*this, MIRROR_SIZE);
		Transform::translate(*this, { -4.0f, 7.5f, TABLE_SIZE.z / 3.0f });

		//_createMirrorReflectiveArea(scene, group, mirrorMesh);

		//MirrorComponent mirrorComponent = addComponent<MirrorComponent>(rigidbody, mirrorMesh);
		// set mirror normal
	}

public: 
	void _createMirrorReflectiveArea(Scene* scene, GroupComponent& group, const MeshComponent& mesh)
	{
		Entity body = group.addNewEntity(scene, *this, "body");
		body.addComponent<MeshComponent>(mesh, body);
		Transform::scale(body, MIRROR_SIZE);
		Transform::translate(body, { -4.0f, 7.5f, TABLE_SIZE.z / 3.0f });

	}

	void _createMirrorBody(Scene* scene, GroupComponent& group, const MeshComponent& mesh)
	{

	}
};

#endif // !__mm3d_entities_world_rearViewMirror__