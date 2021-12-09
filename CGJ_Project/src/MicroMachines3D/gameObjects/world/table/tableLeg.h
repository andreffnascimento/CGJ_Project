#ifndef __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLELEG_H__
#define __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLELEG_H__


#include "engine/renderer/geometry.h"
#include "engine/scene/components/renderable.h"

#include "MicroMachines3D/common/constants.h"




class TableLeg : public Renderable
{

public:
	TableLeg(const Material &material, const std::shared_ptr<MyMesh> &mesh, float xMod, float zMod)
		: Renderable()
	{
		_mesh = mesh;
		setMaterial(material);
		setScale(LEG_THICKNESS, LEG_LENGTH, LEG_THICKNESS);

		float xPos = (xMod * TABLE_LENGTH / 2.0f) - (xMod * LEG_THICKNESS / 2.0f);
		float yPos = -LEG_LENGTH / 2.0f;
		float zPos = (zMod * TABLE_WIDTH / 2.0f) - (zMod * LEG_THICKNESS / 2.0f);
		setPosition(xPos, yPos, zPos);
	}

};


#endif // !__MM3D_GAMEOBJECTS_WORLD_TABLE_TABLELEG_H__
