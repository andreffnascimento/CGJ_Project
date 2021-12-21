#ifndef __MM3D_SCENES_MICROMACHINES3D_H__
#define __MM3D_SCENES_MICROMACHINES3D_H__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/world/cameraManager.h"
#include "MicroMachines3D/world/table.h"




class MicroMachines3DScene : public Scene
{
	
public:
	MicroMachines3DScene() : Scene() {}




private:
	void setupScene() override
	{
		// scene cameras setups
		CameraManager cameraManager = CameraManager(this);

		// world objects setup
		Table table = Table(this);
	}
};


#endif // !__MM3D_SCENES_MICROMACHINES3D_H__