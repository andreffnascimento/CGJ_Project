#ifndef __mm3d_world_table__
#define __mm3d_world_table__


#include "MicroMachines3D/common/include.h"




class Table : public SceneEntity
{

public:
	Table(Scene* scene)
		: SceneEntity(scene->createEntity("Table"))
	{
		Material material = {
			{ 0.2f, 0.15f, 0.1f, 1.0f },
			{ 0.8f, 0.6f, 0.4f, 1.0f },
			{ 0.8f, 0.8f, 0.8f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			100.0f,
			0
		};
		
		std::shared_ptr<MyMesh> mesh = std::make_shared<MyMesh>(createCube());
		MeshComponent::setMaterial(*mesh, material);

		GroupComponent& group = addComponent<GroupComponent>();
		createTop(group, mesh);
		createLeg(group, mesh, -1.0f, -1.0f, "1");
		createLeg(group, mesh,  1.0f, -1.0f, "2");
		createLeg(group, mesh, -1.0f,  1.0f, "3");
		createLeg(group, mesh,  1.0f,  1.0f, "4");
	}


private:
	void createTop(GroupComponent& group, const std::shared_ptr<MyMesh>& mesh)
	{
		Entity top = group.addNewEntity(*this, "top");
		top.addComponent<MeshComponent>(mesh);
		Transform::scale(top, TABLE_SIZE);
		Transform::translate(top, { 0.0f, TABLE_SIZE.y / 2.0f, 0.0f });
	}


	void createLeg(GroupComponent& group, const std::shared_ptr<MyMesh>& mesh, float xMod, float zMod, const char *legId)
	{
		Entity leg = group.addNewEntity(*this, "leg" + std::string(legId));
		leg.addComponent<MeshComponent>(mesh);
		Transform::scale(leg, LEG_SIZE);

		float xPos = (xMod * TABLE_SIZE.x / 2.0f) - (xMod * LEG_SIZE.x / 2.0f);
		float yPos = -LEG_SIZE.y / 2.0f;
		float zPos = (zMod * TABLE_SIZE.z / 2.0f) - (zMod * LEG_SIZE.z / 2.0f);
		Transform::translate(leg, { xPos, yPos, zPos });
	}

};


#endif // !__mm3d_world_table__
