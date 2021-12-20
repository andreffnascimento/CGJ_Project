#ifndef __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLE_H__
#define __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLE_H__


#include "MicroMachines3D/common/include.h"




class Table : public Entity
{

public:
	Table() = delete;
	Table(const Table&) = default;
	~Table() = default;


public:
	Table(const Entity& entity)
		: Entity(entity)
	{
		Material material = {
			{ 0.2f, 0.15f, 0.1f, 1.0f },
			{ 0.8f, 0.6f, 0.4f, 1.0f },
			{ 0.8f, 0.8f, 0.8f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			100.0f,
			0
		};

		//create mesh for table (legs + cover)
		MyMesh *mesh = new MyMesh(createCube());
		MeshComponent::setMaterial(mesh, material);

		GroupComponent& group = entity.addComponent<GroupComponent>();

		createTop(group, mesh);
		createLeg(group, mesh, -1.0f, -1.0f);
		createLeg(group, mesh,  1.0f, -1.0f);
		createLeg(group, mesh, -1.0f,  1.0f);
		createLeg(group, mesh,  1.0f,  1.0f);
	}


private:
	void createTop(GroupComponent& group, MyMesh* mesh)
	{
		Entity top = group.addNewEntity(*this);
		top.addComponent<MeshComponent>(mesh);
		Transform::scale(top, { TABLE_LENGTH, TABLE_THICKNESS, TABLE_WIDTH });
		Transform::translate(top, { 0.0f, TABLE_THICKNESS / 2.0f, 0.0f });
	}


	void createLeg(GroupComponent& group, MyMesh* mesh, float xMod, float zMod)
	{
		Entity leg = group.addNewEntity(*this);
		leg.addComponent<MeshComponent>(mesh);
		Transform::scale(leg, { LEG_THICKNESS, LEG_LENGTH, LEG_THICKNESS });

		float xPos = (xMod * TABLE_LENGTH / 2.0f) - (xMod * LEG_THICKNESS / 2.0f);
		float yPos = -LEG_LENGTH / 2.0f;
		float zPos = (zMod * TABLE_WIDTH / 2.0f) - (zMod * LEG_THICKNESS / 2.0f);
		Transform::translate(leg, { xPos, yPos, zPos });
	}

};


#endif // !__MM3D_GAMEOBJECTS_WORLD_TABLE_TABLE_H__
