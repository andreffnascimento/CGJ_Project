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

		createPlanet(scene, group, 1, 299.0f, 99.0f, 69.0f, 3.0f, "src/MicroMachines3D/textures/planets/sun.png");
		createPlanet(scene, group, 2, -299.0f, 99.0f, 69.0f, 1.0f, "src/MicroMachines3D/textures/planets/earth.png");
		createPlanet(scene, group, 3, -299.0f, 99.0f, 169.0f, 1.0f, "src/MicroMachines3D/textures/planets/mars.png");
		createPlanet(scene, group, 4, 299.0f, 99.0f, 169.0f, 2.0f, "src/MicroMachines3D/textures/planets/saturn.png");
		createPlanet(scene, group, 5, 199.0f, 99.0f, 69.0f, 2.0f, "src/MicroMachines3D/textures/planets/jupiter.png");
		createPlanet(scene, group, 6, -199.0f, 99.0f, 69.0f, 2.0f, "src/MicroMachines3D/textures/planets/neptune.png");
		createPlanet(scene, group, 7, 199.0f, 99.0f, 169.0f, 0.7f, "src/MicroMachines3D/textures/planets/pluto.png");
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