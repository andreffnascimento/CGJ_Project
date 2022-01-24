#ifndef __mm3d_entities_particles_fireworks__
#define __mm3d_entities_particles_fireworks__


#include "MicroMachines3D/common/include.h"




class Fireworks : public SceneEntity
{

public:
	Fireworks(Scene* scene)
		: SceneEntity(scene->createEntity("Fireworks"))
	{
		unsigned int fireworksTextureId = Renderer::create2dTexture("src/MicroMachines3D/textures/fireworks/fireworks.tga");
		ParticleGeneratorComponent& particleGenerator = addComponent<ParticleGeneratorComponent>(PARTICLE_SIZE, fireworksTextureId);
		particleGenerator.initializeParticleStorage(N_PARTICLES);
	}

};


#endif // !__mm3d_entities_particles_fireworks__