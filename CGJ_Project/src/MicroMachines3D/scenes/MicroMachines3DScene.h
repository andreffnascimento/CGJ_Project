#ifndef __mm3d_scenes_MicroMachines3D__
#define __mm3d_scenes_MicroMachines3D__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/entities/manager/gameManager.h"
#include "MicroMachines3D/entities/manager/cameraManager.h"
#include "MicroMachines3D/entities/world/table.h"
#include "MicroMachines3D/entities/world/car.h"
#include "MicroMachines3D/entities/world/cheeriosTrack.h"
#include "MicroMachines3D/entities/world/candels.h"




class MicroMachines3DScene : public Scene
{
	
public:
	MicroMachines3DScene() : Scene() {}




private:
	void setupScene() override
	{
		setReflectionCoefficients(0.5f, 1.0f, 1.0f);
		
		GameManager gameManager = GameManager(this);
		CameraManager cameraManager = CameraManager(this);

		Table table = Table(this);
		Transform::translate(table, { 0.0f, -TABLE_SIZE.y, 0.0f });

		Car car = Car(this);
		Transform::translateTo(car, { 0.0f, CAR_BOTTOM_SIZE.y / 2.0f + CAR_FLOOR_DISTANCE + CAR_SPAWN_HEIGHT, TABLE_SIZE.z / 3.0f });
		Transform::rotate(car, { 0.0f, 90.0f, 0.0f });

		CheeriosTrack cheeriosTrack = CheeriosTrack(this);

		Candels candels = Candels(this);
	}
};


#endif // !__mm3d_scenes_MicroMachines3D__