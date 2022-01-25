#ifndef __mm3d_entities_world_rearViewMirror__
#define __mm3d_entities_world_rearViewMirror__


#include "MicroMachines3D/common/include.h"



class RearViewMirror : public SceneEntity
{

public:
	RearViewMirror(Scene* scene)
		: SceneEntity(scene->createEntity("RearViewMirror"))
	{
		GroupComponent& group = addComponent<GroupComponent>();

		MeshComponent mirrorMesh = MeshComponent(createCube(), TABLE_MATERIAL);
		mirrorMesh.setIsMirror(true);
		_createMirrorReflectiveArea(scene, group, mirrorMesh);
	}

public: 
	void _createMirrorReflectiveArea(Scene* scene, GroupComponent& group, const MeshComponent& mesh)
	{
		Entity body = group.addNewEntity(scene, *this, "body");
		body.addComponent<MeshComponent>(mesh, body);
		Transform::scale(body, MIRROR_SIZE);
		Transform::translate(body, { 2.0f, 4.5f, TABLE_SIZE.z / 3.0f });

	}

	void _createMirrorBody(Scene* scene, GroupComponent& group, const MeshComponent& mesh)
	{

	}
};

#endif // !__mm3d_entities_world_rearViewMirror__