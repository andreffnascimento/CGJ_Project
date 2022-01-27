#ifndef __mm3d_entities_world_lensFlare__
#define __mm3d_entities_world_lensFlare__

#include "MicroMachines3D/common/include.h"

class LensFlare : public SceneEntity 
{

public:
	LensFlare(Scene* scene)
		: SceneEntity(scene->createEntity("LensFlare"))
	{
		createLensFlare(scene);
	}


	void createLensFlare(Scene* scene) {
		//LensFlareComponent& flare = addComponent<LensFlareComponent>();
	}
};


#endif // !__mm3d_entities_world_lensFlare__