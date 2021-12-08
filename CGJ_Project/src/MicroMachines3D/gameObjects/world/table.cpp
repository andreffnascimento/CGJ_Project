#include "table.h"

#include "engine/renderer/geometry.h"

#include "MicroMachines3D/common/constants.h"




Table::Table()
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
	_mesh = createCube();
	setMaterial(material);
	setPosition(0.0f, TABLE_THICKNESS / 2.0f, 0.0f);
	setScale(TABLE_LENGTH, TABLE_THICKNESS, TABLE_WIDTH);
}