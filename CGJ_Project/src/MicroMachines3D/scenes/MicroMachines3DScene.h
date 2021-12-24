#ifndef __mm3d_scenes_MicroMachines3D__
#define __mm3d_scenes_MicroMachines3D__


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
		setReflectionCoefficients(0.5f, 1.0f, 1.0f);
		
		CameraManager cameraManager = CameraManager(this);


		Table table = Table(this);
		Transform::translate(table, { 0.0f, -TABLE_SIZE.y, 0.0f });

		Car car = Car(this);
		Transform::translate(car, { -TABLE_SIZE.x / 2.2f, CAR_BOTTOM_SIZE.y / 2.0f + CAR_FLOOR_DISTANCE, TABLE_SIZE.z / 2.2f });
		Transform::rotate(car, { 0.0f, 90.0f, 0.0f });

		// Temporary point light
		Entity light = createEntity("dirlight");
		LightComponent& lightComponent = light.addComponent<LightComponent>();
		lightComponent.setDirectionalLight({1.0f, -1.0f, -1.0f}, 2.0f);
	}
};


#endif // !__mm3d_scenes_MicroMachines3D__