#ifndef __mm3d_entities_world_boosters__
#define __mm3d_entities_world_boosters__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/boosters/boostersDriftScript.h"




class Boosters : public SceneEntity
{

public:
	Boosters(Scene* scene)
		: SceneEntity(scene->createEntity("Boosters"))
	{
		GroupComponent& group = addComponent<GroupComponent>();
		MeshComponent mesh = MeshComponent(createCube(), BOOSTER_MATERIAL);

		_createBooster(scene, group, mesh, 45.0f, 25.0f, 15.0f, 08.0f);
		_createBooster(scene, group, mesh, 30.0f, 25.0f, 15.0f, 06.0f);
		_createBooster(scene, group, mesh, 15.0f, 25.0f, 15.0f, 04.0f);

		addComponent<ScriptComponent>(std::make_shared<BoostersDriftScript>(scene));
	}




private:
	void _createBooster(Scene* scene, GroupComponent& group, const MeshComponent& mesh, float xPos, float zPos, float xSize, float zSize)
	{
		Entity booster = group.addNewEntity(scene, *this);
		booster.addComponent<MeshComponent>(mesh, booster);

		RigidbodyComponent& rigidbody = booster.addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::STATIC);
		AABBColliderComponent& collider = booster.addComponent<AABBColliderComponent>(booster, ColliderIds::BOOSTER, rigidbody, Coords3f({ xSize, BUTTER_HEIGHT, zSize }));
		collider.setFixedBoundingBox(true);

		Transform::translate(booster, Coords3f({ xPos, BUTTER_HEIGHT / 2.0f, zPos }));
		Transform::scale(booster, Coords3f({ xSize, BUTTER_HEIGHT, zSize }));
	}

};


#endif // !__mm3d_entities_world_Boosters__