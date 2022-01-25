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
		
		Entity planet = group.addNewEntity(scene, *this, "neptune");

		ImageComponent& image = planet.addComponent<ImageComponent>(planet, ImageComponent::ImageType::FAST_CYLINDRICAL_BILLBOARD);

		image.addImage(Renderer::create2dTexture("src/MicroMachines3D/textures/planets/neptune.png"));


		Transform::scaleTo(planet, { 20.0f, 20.0f, 1.0f } );
		Transform::translate(planet, { 0.0f, 0.0f, 0.0f });
	}

};


#endif // !__mm3d_entities_world_planets__
