#ifndef __mm3d_entities_world_butters__
#define __mm3d_entities_world_butters__


#include "MicroMachines3D/common/include.h"


class Butters : public SceneEntity
{

public:
	Butters(Scene* scene)
		: SceneEntity(scene->createEntity("Butters"))
	{
		GroupComponent& group = addComponent<GroupComponent>();

		MeshComponent mesh = MeshComponent(createCube(), BUTTER_MATERIAL);
		
		_createButter(scene, group, mesh,  40.0f,  00.0f, 05.0f, 25.0f);
		_createButter(scene, group, mesh,  10.0f, -15.0f, 10.0f, 07.0f);
		_createButter(scene, group, mesh,  10.0f,  13.0f, 30.0f, 03.0f);
		_createButter(scene, group, mesh, -25.0f,  07.0f, 20.0f, 07.0f);
		_createButter(scene, group, mesh, -15.0f, -17.0f, 13.0f, 23.0f);

		_createButter(scene, group, mesh, -62.0f, -13.0f, 06.0f, 04.0f);
		_createButter(scene, group, mesh, -68.0f,  00.0f, 06.0f, 04.0f);
		_createButter(scene, group, mesh, -62.0f,  13.0f, 06.0f, 04.0f);

		_createButter(scene, group, mesh, -45.0f,  20.0f, 15.0f, 02.0f);
		_createButter(scene, group, mesh, -45.0f,  30.0f, 15.0f, 02.0f);
		_createButter(scene, group, mesh, -30.0f,  20.5f, 15.0f, 03.0f);
		_createButter(scene, group, mesh, -30.0f,  29.5f, 15.0f, 03.0f);
		_createButter(scene, group, mesh, -15.0f,  21.0f, 15.0f, 04.0f);
		_createButter(scene, group, mesh, -15.0f,  29.0f, 15.0f, 04.0f);
	}




private:
	void _createButter(Scene* scene, GroupComponent& group, const MeshComponent& mesh, float xPos, float zPos, float xSize, float zSize)
	{
		Entity butter = group.addNewEntity(scene, *this);
		butter.addComponent<MeshComponent>(mesh, butter);

		RigidbodyComponent& rigidbody = butter.addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::STATIC);

		Transform::translate(butter, Coords3f({ xPos, BUTTER_HEIGHT / 2.0f, zPos }));
		Transform::scale(butter, Coords3f({ xSize, BUTTER_HEIGHT, zSize }));
	}

};


#endif // !__mm3d_entities_world_cheeriosTrack__