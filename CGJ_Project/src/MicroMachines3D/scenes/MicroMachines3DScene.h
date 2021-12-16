#ifndef __MM3D_SCENES_MICROMACHINES3D_H__
#define __MM3D_SCENES_MICROMACHINES3D_H__


#include "engine/scene/scene.h"
#include "engine/math/transform.h"




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
		camera.setPerspectiveCamera({ 0.01f, 1000.0f }, 60.0f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { -50, 0, 0 });
		setActiveCamera(Camera(entity));
	}
};


#endif // !__MM3D_SCENES_MICROMACHINES3D_H__