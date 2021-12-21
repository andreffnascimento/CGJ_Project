#ifndef __MM3D_WORLD_CAR_H__
#define __MM3D_WORLD_CAR_H__


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

		Entity carSeats = group.addNewEntity(*this, "seats");
		carSeats.addComponent<MeshComponent>(mesh);
		Transform::scale(carSeats, { CAR_BOTTOM_WIDTH, CAR_BOTTOM_HEIGHT, CAR_BOTTOM_LENGTH });

		Entity carFront = group.addNewEntity(*this, "front");
		carFront.addComponent<MeshComponent>(mesh);
		Transform::scale(carFront, { CAR_TOP_WIDTH, CAR_TOP_HEIGHT, CAR_TOP_LENGTH });
		Transform::translate(carFront, { 0.0f, CAR_BOTTOM_HEIGHT, CAR_BOTTOM_WIDTH / 2.0f });
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
	}

};


#endif // !__MM3D_WORLD_TABLE_H__
