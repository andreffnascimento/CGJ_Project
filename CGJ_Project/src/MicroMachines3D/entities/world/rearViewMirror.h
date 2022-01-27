#ifndef __mm3d_entities_world_rearViewMirror__
#define __mm3d_entities_world_rearViewMirror__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/mirror/MirrorMovementScript.h"


class RearViewMirror : public SceneEntity
{

public:
	RearViewMirror(Scene* scene)
		: SceneEntity(scene->createEntity("RearViewMirror"))
	{	
		WindowCoords windowCoords = Application::getInstance().getOriginalWindowSize();
		FixedMirrorComponent fixedMirrorComponent = addComponent<FixedMirrorComponent>(createCube(), MIRROR_MATERIAL);
		fixedMirrorComponent.setScreenPositionX(0);
		fixedMirrorComponent.setScreenPositionY(0);

		Transform::rotate(*this, {0.0, 0.0, 0,0});
		Transform::scale(*this, MIRROR_SIZE);
		Transform::translate(*this, { windowCoords.x / 2.0f, windowCoords.y - MIRROR_SIZE.y / 2.0f, 0.0f });

		//Transform::translate(*this, Coords3f{ 0.0f, 1.0f, 3.0f });
		//Transform::translate(*this, { -4.0f, 7.5f, TABLE_SIZE.z / 3.0f });

		addComponent<ScriptComponent>(std::make_shared<MirrorMovementScript>(scene));
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