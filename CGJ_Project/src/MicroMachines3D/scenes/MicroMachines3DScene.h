#ifndef __MM3D_SCENES_MICROMACHINES3D_H__
#define __MM3D_SCENES_MICROMACHINES3D_H__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/world/cameraManager.h"
#include "MicroMachines3D/world/table.h"
#include "MicroMachines3D/world/car.h"




class MicroMachines3DScene : public Scene
{
	
public:
	MicroMachines3DScene() : Scene() {}




private:
	void setupScene() override
	{
		CameraManager cameraManager = CameraManager(this);


		Table table = Table(this);
		Transform::translate(table, { 0.0f, -TABLE_THICKNESS, 0.0f });

		Car car = Car(this);
	}
};


#endif // !__MM3D_SCENES_MICROMACHINES3D_H__