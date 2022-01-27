#ifndef __mm3d_entities_world_planets__
#define __mm3d_entities_world_planets__


#include "MicroMachines3D/common/include.h"

class Planets : public SceneEntity
{

public:
	Planets(Scene* scene)
		: SceneEntity(scene->createEntity("Planets"))
	{
		GroupComponent& group = addComponent<GroupComponent>();

		bigBang(scene, group);

	}

	void bigBang(Scene* scene, GroupComponent& group) {

		createPlanet(scene, group, 1, 0.0f, 300.0f, -500.0f, 25.0f, "src/MicroMachines3D/textures/planets/sun.png");
		createPlanet(scene, group, 2, -175.0f, 15.0f, 15.0f, 3.0f, "src/MicroMachines3D/textures/planets/earth.png");
		createPlanet(scene, group, 3, -300.0f, 30.0f, -65.0f, 2.5f, "src/MicroMachines3D/textures/planets/mars.png");
		createPlanet(scene, group, 4, 350.0f, 10.0f, -80.0f, 9.0f, "src/MicroMachines3D/textures/planets/jupiter.png");
		createPlanet(scene, group, 5, 450.0f, 30.0f, 80.0f, 8.0f, "src/MicroMachines3D/textures/planets/saturn.png");
		createPlanet(scene, group, 6, 100.0f, 40.0f, 300.0f, 6.5f, "src/MicroMachines3D/textures/planets/neptune.png");
		createPlanet(scene, group, 7, -50.0f, 20.0f, 100.0f, 1.5f, "src/MicroMachines3D/textures/planets/pluto.png");
	}

	void createPlanet(Scene* scene, GroupComponent& group, int id, float posX, float posY, float posZ, float sizeCoeficient, const char* texturePath) {
		Entity planet = group.addNewEntity(scene, *this, "planet_" + std::to_string(id));

		ImageComponent& image = planet.addComponent<ImageComponent>(planet, ImageComponent::ImageType::FAST_CYLINDRICAL_BILLBOARD);
		image.addImage(Renderer::create2dTexture(texturePath));

		Transform::scaleTo(planet, { 25.0f * sizeCoeficient, 25.0f * sizeCoeficient, 1.0f });
		Transform::translate(planet, { posX, posY, posZ });
	}
};


#endif // !__mm3d_entities_world_planets__