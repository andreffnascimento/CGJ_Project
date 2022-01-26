#ifndef __mm3d_entities_world_skybox__
#define __mm3d_entities_world_skybox__


#include "MicroMachines3D/common/include.h"




class Skybox : public SceneEntity
{

public:
	Skybox(Scene* scene)
		: SceneEntity(scene->createEntity("Skybox"))
	{
		const char* skyboxPaths[] = {
			"src/MicroMachines3D/textures/skybox/right.png",
			"src/MicroMachines3D/textures/skybox/left.png",
			"src/MicroMachines3D/textures/skybox/top.png",
			"src/MicroMachines3D/textures/skybox/bottom.png",
			"src/MicroMachines3D/textures/skybox/front.png",
			"src/MicroMachines3D/textures/skybox/back.png"
		};

		SkyboxComponent& skybox = addComponent<SkyboxComponent>(Renderer::createCubeMapTexture(skyboxPaths));
		Transform::scaleTo(*this, SKYBOX_SIZE);
		Renderer::setSkybox(*this);
	}

};


#endif // !__mm3d_entities_world_skybox__