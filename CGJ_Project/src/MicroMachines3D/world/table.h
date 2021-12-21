#ifndef __MM3D_WORLD_TABLE_H__
#define __MM3D_WORLD_TABLE_H__


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
		Transform::scale(top, { TABLE_LENGTH, TABLE_THICKNESS, TABLE_WIDTH });
		Transform::translate(top, { 0.0f, TABLE_THICKNESS / 2.0f, 0.0f });
	}


	void createLeg(GroupComponent& group, const std::shared_ptr<MyMesh>& mesh, float xMod, float zMod, const char *legId)
	{
		Entity leg = group.addNewEntity(*this, "leg" + std::string(legId));
		leg.addComponent<MeshComponent>(mesh);
		Transform::scale(leg, { LEG_THICKNESS, LEG_LENGTH, LEG_THICKNESS });

		float xPos = (xMod * TABLE_LENGTH / 2.0f) - (xMod * LEG_THICKNESS / 2.0f);
		float yPos = -LEG_LENGTH / 2.0f;
		float zPos = (zMod * TABLE_WIDTH / 2.0f) - (zMod * LEG_THICKNESS / 2.0f);
		Transform::translate(leg, { xPos, yPos, zPos });
	}

};


#endif // !__MM3D_WORLD_TABLE_H__
