#ifndef __mm3d_entities_world_rearViewMirror__
#define __mm3d_entities_world_rearViewMirror__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/mirror/rearViewMirrorMovementScript.h"




class RearViewMirror : public SceneEntity
{

public:
	RearViewMirror(Scene* scene)
		: SceneEntity(scene->createEntity("RearViewMirror"))
	{	
		WindowCoords windowCoords = Application::getInstance().getOriginalWindowSize();
		FlatMirrorComponent& flatMirrorComponent = addComponent<FlatMirrorComponent>(*this, createCube(), MIRROR_MATERIAL);

		Entity camera = scene->createEntity("RearViewCamera");
		camera.addComponent<CameraComponent>(CameraComponent::CameraProjection::PERSPECTIVE, ClippingPlanes({ 0.01f, 1000.0f }), 70.0f);
		flatMirrorComponent.setCamera(camera);
		flatMirrorComponent.setEnabled(false);

		Transform::scale(*this, MIRROR_SIZE);
		Transform::translateTo(*this, { windowCoords.x / 2.0f , windowCoords.y - MIRROR_SIZE.y / 2.0f, 0.0f });

		addComponent<ScriptComponent>(std::make_shared<RearViewMirrorMovementScript>(scene));
	}

};


#endif // !__mm3d_entities_world_rearViewMirror__