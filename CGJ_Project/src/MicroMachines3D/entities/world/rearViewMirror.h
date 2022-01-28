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

		Transform::rotate(*this, {0.0, 0.0, 0,0});
		Transform::scale(*this, MIRROR_SIZE);
		Transform::translate(*this, { windowCoords.x / 2.0f, windowCoords.y - MIRROR_SIZE.y / 2.0f, 0.0f });

		addComponent<ScriptComponent>(std::make_shared<MirrorMovementScript>(scene));
	}

};

#endif // !__mm3d_entities_world_rearViewMirror__