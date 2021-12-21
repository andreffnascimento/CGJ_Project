#ifndef __mm3d_world_car__
#define __mm3d_world_car__


#include "MicroMachines3D/common/include.h"




class Car : public SceneEntity
{

public:
	Car(Scene* scene)
		: SceneEntity(scene->createEntity("Car"))
	{
		GroupComponent& group = addComponent<GroupComponent>();

		_initCarBody(group);
		_initCarWheels(group);
	}


private:
	void _initCarBody(GroupComponent& group)
	{
		Material material = {
			{ 0.5f, 110.0f / 510.0f, 25.0f / 510.0f, 1.0f },
			{ 1.0f, 110.0f / 255.0f, 25.0f / 255.0f, 1.0f },
			{ 1.0f, 110.0f / 255.0f, 25.0f / 255.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			100.0f,
			0
		};

		std::shared_ptr<MyMesh> mesh = std::make_shared<MyMesh>(createCube());
		MeshComponent::setMaterial(*mesh, material);

		Entity carBottom = group.addNewEntity(*this, "bottom");
		carBottom.addComponent<MeshComponent>(mesh);
		Transform::scale(carBottom, { CAR_BOTTOM_WIDTH, CAR_BOTTOM_HEIGHT, CAR_BOTTOM_LENGTH });

		Entity carTop = group.addNewEntity(*this, "top");
		carTop.addComponent<MeshComponent>(mesh);
		Transform::scale(carTop, { CAR_TOP_WIDTH, CAR_TOP_HEIGHT, CAR_TOP_LENGTH });
		Transform::translate(carTop, { 0.0f, (CAR_BOTTOM_HEIGHT + CAR_TOP_HEIGHT) / 2.0f, 0.0f });
	}


	void _initCarWheels(GroupComponent& group)
	{
		Material material = {
			{ 0.1f, 0.1f, 0.1f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			20.0f,
			0
		};

		std::shared_ptr<MyMesh> mesh = std::make_shared<MyMesh>(createTorus(WHEEL_INNER_RADIUS, WHEEL_OUTER_RADIUS, WHEEL_RINGS, WHEEL_SIDES));
		MeshComponent::setMaterial(*mesh, material);

		_initCarWheel(group, mesh,  1.0f,  1.0f, "1");
		_initCarWheel(group, mesh, -1.0f,  1.0f, "2");
		_initCarWheel(group, mesh, -1.0f, -1.0f, "3");
		_initCarWheel(group, mesh,  1.0f, -1.0f, "4");
	}


	void _initCarWheel(GroupComponent& group, const std::shared_ptr<MyMesh>& mesh, float xMod, float zMod, const char* wheelId)
	{
		Entity wheel = group.addNewEntity(*this, "wheel" + std::string(wheelId));
		wheel.addComponent<MeshComponent>(mesh);

		Transform::rotate(wheel, { 0.0f, 0.0f, 90.0f });
		Transform::scale(wheel, { WHEEL_WIDTH, WHEEL_HEIGHT, WHEEL_LENGTH });

		float xPos = (xMod * CAR_BOTTOM_WIDTH / 2.0f) + (xMod * WHEEL_WIDTH / 2.0f);
		float zPos = (zMod * CAR_BOTTOM_LENGTH / 2.0f) - (zMod * WHEEL_LENGTH);
		Transform::translate(wheel, { xPos, 0.0f, zPos });
	}

};


#endif // !__mm3d_world_car__
