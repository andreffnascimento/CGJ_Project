#ifndef __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLETOP_H__
#define __MM3D_GAMEOBJECTS_WORLD_TABLE_TABLETOP_H__


#include "engine/renderer/geometry.h"
#include "engine/scene/components/renderable.h"

#include "MicroMachines3D/common/constants.h"




class TableTop : public Renderable
{

public:
	TableTop(const Material& material, const std::shared_ptr<MyMesh>& mesh)
		: Renderable()
	{
		_mesh = mesh;
		setMaterial(material);
		setScale(TABLE_LENGTH, TABLE_THICKNESS, TABLE_WIDTH);
		setPosition(0.0f, -TABLE_THICKNESS / 2.0f, 0.0f);
	}

};


#endif // !__MM3D_GAMEOBJECTS_WORLD_TABLE_TABLETOP_H__
