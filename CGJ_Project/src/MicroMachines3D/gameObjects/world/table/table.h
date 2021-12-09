#ifndef __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLE_H__
#define __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLE_H__


#include "engine/renderer/geometry.h"
#include "engine/scene/components/renderable.h"

#include "MicroMachines3D/common/constants.h"
#include "MicroMachines3D/gameObjects/world/table/tableTop.h"
#include "MicroMachines3D/gameObjects/world/table/tableLeg.h"




class Table : public Renderable
{

public:
	Table()
		: Renderable()
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
		_mesh = std::shared_ptr<MyMesh>(new MyMesh(createCube()));
		
		TableTop *top = new TableTop(material, _mesh);
		addChild(top);

		TableLeg *leg1 = new TableLeg(material, _mesh, -1.0f, -1.0f);
		addChild(leg1);

		TableLeg *leg2 = new TableLeg(material, _mesh, 1.0f, -1.0f);
		addChild(leg2);

		TableLeg *leg3 = new TableLeg(material, _mesh, -1.0f, 1.0f);
		addChild(leg3);

		TableLeg *leg4 = new TableLeg(material, _mesh, 1.0f, 1.0f);
		addChild(leg4);
	}

};


#endif // !__MM3D_GAMEOBJECTS_WORLD_TABLE_TABLE_H__
