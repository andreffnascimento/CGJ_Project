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
		camera.setPerspectiveCamera({ 0.01f, 1000.0f }, 53.13f);
		camera.setTargetCoords({ 0.0f, 0.0f, 0.0f });
		Transform::translateTo(entity, { 30.0f, 50.0f, 30.0f });
		setActiveCamera(Camera(entity));

		// temporary object
		Material material = {
			{ 0.2f, 0.15f, 0.1f, 1.0f },
			{ 0.8f, 0.6f, 0.4f, 1.0f },
			{ 0.8f, 0.8f, 0.8f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			100.0f,
			0
		};

		Entity cube = createEntity();
		MeshComponent& mesh = cube.addComponent<MeshComponent>(new MyMesh(createCube()));
		Transform::scaleTo(cube, { 10.0f, 2.5f, 10.0f });
		mesh.setMaterial(material);
		
	}
};


#endif // !__MM3D_SCENES_MICROMACHINES3D_H__