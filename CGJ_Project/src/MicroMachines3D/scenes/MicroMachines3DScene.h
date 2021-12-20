#ifndef __MM3D_SCENES_MICROMACHINES3D_H__
#define __MM3D_SCENES_MICROMACHINES3D_H__


#include "MicroMachines3D/world/table.h"




class MicroMachines3DScene : public Scene
{
	
public:
	MicroMachines3DScene() : Scene() {}




private:
	void onCreate() override
	{
		// temporary test camera
		Entity entity = createEntity();
		CameraComponent& camera = entity.addComponent<CameraComponent>();
		camera.setPerspectiveCamera({ 0.01f, 1000.0f }, 53.13f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { 100.0f, 10.0f, 170.0f });
		setActiveCamera(Camera(entity));
		// temporaty test end


		Table table = Table(createEntity());
		Transform::rotate(table, { 30.0f, 0.0f, 0.0f });

	}
};


#endif // !__MM3D_SCENES_MICROMACHINES3D_H__