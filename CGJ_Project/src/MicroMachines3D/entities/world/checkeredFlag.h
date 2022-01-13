#ifndef __mm3d_entities_world_checkeredFlag__
#define __mm3d_entities_world_checkeredFlag__


#include "MicroMachines3D/common/include.h"


class CheckeredFlag : public SceneEntity
{

public:
	CheckeredFlag(Scene* scene)
		: SceneEntity(scene->createEntity("flags"))
	{
		GroupComponent& group = addComponent<GroupComponent>();
		MeshComponent mesh = MeshComponent(createCube(2, 9), FLAG_MATERIAL);
		mesh.setTextureMode(Texture::TextureMode::MODULATE_DIFFUSE);
		mesh.addTexture(Renderer::create2dTexture("src/MicroMachines3D/textures/flag/flag.jpg"));

		_createFlag(scene, group, mesh, -2.0f * CAR_BOTTOM_SIZE.x, TABLE_SIZE.z / 3.0f, 2.0f * CAR_BUMPER_SIZE.x, TRACK_WIDTH);

	}




private:
	void _createFlag(Scene* scene, GroupComponent& group, const MeshComponent& mesh, float xPos, float zPos, float xSize, float zSize)
	{
		Entity flag = group.addNewEntity(scene, *this);
		flag.addComponent<MeshComponent>(mesh, flag);

		Transform::translate(flag, Coords3f({ xPos, BUTTER_HEIGHT / 4.0f, zPos }));
		Transform::scale(flag, Coords3f({ xSize, BUTTER_HEIGHT / 2.0f, zSize }));
	}

};


#endif // !__mm3d_entities_world_checkeredFlag__