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

		createPlanet(scene, group, 1, PLANET_SUN_POSITION, PLANET_SUN_SIZE, "src/MicroMachines3D/textures/planets/sun.png");
		createPlanet(scene, group, 2, PLANET_EARTH_POSITION, PLANET_EARTH_SIZE, "src/MicroMachines3D/textures/planets/earth.png");
		createPlanet(scene, group, 3, PLANET_MARS_POSITION, PLANET_MARS_SIZE, "src/MicroMachines3D/textures/planets/mars.png");
		createPlanet(scene, group, 4, PLANET_JUPITER_POSITION, PLANET_JUPITER_SIZE, "src/MicroMachines3D/textures/planets/jupiter.png");
		createPlanet(scene, group, 5, PLANET_SATURN_POSITION, PLANET_SATURN_SIZE,"src/MicroMachines3D/textures/planets/saturn.png");
		createPlanet(scene, group, 6, PLANET_NEPTUNE_POSITION, PLANET_NEPTUNE_SIZE, "src/MicroMachines3D/textures/planets/neptune.png");
		createPlanet(scene, group, 7, PLANET_PLUTO_POSITION, PLANET_PLUTO_SIZE, "src/MicroMachines3D/textures/planets/pluto.png");
	}

	void createPlanet(Scene* scene, GroupComponent& group, int id, const Coords3f& pos, float sizeCoeficient, const char* texturePath) {
		Entity planet = group.addNewEntity(scene, *this, "planet_" + std::to_string(id));

		ImageComponent& image = planet.addComponent<ImageComponent>(planet, ImageComponent::ImageType::FAST_SPHERICAL_BILLBOARD);
		image.addImage(Renderer::create2dTexture(texturePath));

		Transform::scaleTo(planet, { 25.0f * sizeCoeficient, 25.0f * sizeCoeficient, 1.0f });
		Transform::translate(planet, { pos.x, pos.y, pos.z });
	}
};


#endif // !__mm3d_entities_world_planets__