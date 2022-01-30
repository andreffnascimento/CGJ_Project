#ifndef __mm3d_entities_world_floatingCube__
#define __mm3d_entities_world_floatingCube__


#include "MicroMachines3D/common/include.h"




class FloatingCube : public SceneEntity
{

public:
	FloatingCube(Scene* scene)
		: SceneEntity(scene->createEntity("FloatingCube"))
	{

		MeshComponent mesh = MeshComponent(createCube(7, 7), TABLE_MATERIAL);
		mesh.setTextureMode(Texture::TextureMode::CUBE_MAPPING);
		

		GroupComponent& group = addComponent<GroupComponent>();
		createTop(scene, group, mesh);

		Transform::translate(*this, { 0.0f, 3.0f * TABLE_SIZE.y, 0.0f });
		Transform::scale(*this, 2.0f * FLOATER_SIZE);

	}



private:
	void createTop(Scene* scene, GroupComponent& group, const MeshComponent& mesh)
	{
		Entity cube = group.addNewEntity(scene, *this, "cube");
		cube.addComponent<MeshComponent>(mesh, cube);
	}


};


#endif // !__mm3d_entities_world_floatingCube__
