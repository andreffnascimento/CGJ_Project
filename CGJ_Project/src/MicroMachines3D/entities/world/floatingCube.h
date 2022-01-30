#ifndef __mm3d_entities_world_floatingCube__
#define __mm3d_entities_world_floatingCube__


#include "MicroMachines3D/common/include.h"




class FloatingCube : public SceneEntity
{

public:
	FloatingCube(Scene* scene)
		: SceneEntity(scene->createEntity("FloatingCube"))
	{
		unsigned int skyboxColorMap = scene->getEntityByTag("Skybox").getComponent<SkyboxComponent>().texture().colorMapIds()[0];
		unsigned int lightwoodColorMap = scene->getEntityByTag("Table:top").getComponent<MeshComponent>().texture().colorMapIds()[0];

		MeshComponent& mesh = addComponent<MeshComponent>(createCube(), FLOATING_CUBE_MATERIAL, *this);
		mesh.setTextureMode(Texture::TextureMode::CUBE_ENVIRONMENTAL_MAPPING);
		mesh.addColorMap(skyboxColorMap);		// first texture is required to be the cube map
		mesh.addColorMap(lightwoodColorMap);	// second texture is required to be a normal texture

		Transform::translate(*this, { 0.0f, 3.0f * TABLE_SIZE.y, 0.0f });
		Transform::scale(*this, 2.0f * FLOATER_SIZE);
	}

};


#endif // !__mm3d_entities_world_floatingCube__
