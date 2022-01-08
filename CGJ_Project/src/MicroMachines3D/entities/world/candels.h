#ifndef __mm3d_entities_world_candels__
#define __mm3d_entities_world_candels__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/candels/candelsLightScript.h"




class Candels : public SceneEntity
{

public:
	Candels(Scene* scene)
		: SceneEntity(scene->createEntity("Candels"))
	{
		GroupComponent& group = addComponent<GroupComponent>();

		MeshComponent bottomMesh = MeshComponent(createCylinder(CANDEL_HEIGHT, CANDEL_RADIUS, CANDEL_SIDES), CANDEL_OFF_MATERIAL);
		MeshComponent topMesh = MeshComponent(createCube(), CANDEL_OFF_MATERIAL);

		_createCandel(scene, group, bottomMesh, -1.0f, -1.0f, "topLeft");
		_createCandel(scene, group, bottomMesh,  0.0f, -1.0f, "topMiddle");
		_createCandel(scene, group, bottomMesh,  1.0f, -1.0f, "topRight");
		_createCandel(scene, group, bottomMesh, -1.0f,  1.0f, "bottomLeft");
		_createCandel(scene, group, bottomMesh,  0.0f,  1.0f, "bottomMiddle");
		_createCandel(scene, group, bottomMesh,  1.0f,  1.0f, "bottomRight");

		_createCandelLight(scene, group, topMesh, -1.0f, -1.0f, "light_topLeft");
		_createCandelLight(scene, group, topMesh,  0.0f, -1.0f, "light_topMiddle");
		_createCandelLight(scene, group, topMesh,  1.0f, -1.0f, "light_topRight");
		_createCandelLight(scene, group, topMesh, -1.0f,  1.0f, "light_bottomLeft");
		_createCandelLight(scene, group, topMesh,  0.0f,  1.0f, "light_bottomMiddle");
		_createCandelLight(scene, group, topMesh,  1.0f,  1.0f, "light_bottomRight");

		addComponent<ScriptComponent>(std::make_shared<CandelsLightScript>(scene));
	}




private:
	void _createCandel(Scene* scene, GroupComponent& group, const MeshComponent& mesh, float xMod, float zMod, const char* candelId)
	{
		Entity candel = group.addNewEntity(scene, *this, candelId);
		candel.addComponent<MeshComponent>(mesh, candel);

		RigidbodyComponent& rigidbody = candel.addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::STATIC);
		AABBColliderComponent& collider = candel.addComponent<AABBColliderComponent>((unsigned int)ColliderIds::CANDEL, rigidbody, CANDEL_SIZE);
		collider.setFixedBoundingBox(true);

		float xPos = (TABLE_SIZE.x - CANDEL_SIZE.x) * xMod / 2.0f;
		float yPos = CANDEL_SIZE.y / 2.0f;
		float zPos = (TABLE_SIZE.z - CANDEL_SIZE.z) * zMod / 2.0f;

		Transform::scale(candel, CANDEL_SIZE);
		Transform::translate(candel, Coords3f({ xPos, yPos, zPos }));
	}


	void _createCandelLight(Scene* scene, GroupComponent& group, const MeshComponent& mesh, float xMod, float zMod, const char* candelId)
	{
		Entity candel = group.addNewEntity(scene, *this, candelId);
		candel.addComponent<MeshComponent>(mesh, candel);

		LightComponent& light = candel.addComponent<LightComponent>(LightComponent::LightType::POINT, 30.0f);
		light.setEnabled(false);

		float xPos = (TABLE_SIZE.x - CANDEL_SIZE.x) * xMod / 2.0f;
		float yPos = CANDEL_SIZE.y + CANDEL_TOP_SIZE.y / 2.0f;
		float zPos = (TABLE_SIZE.z - CANDEL_SIZE.z) * zMod / 2.0f;

		Transform::scale(candel, CANDEL_TOP_SIZE);
		Transform::translate(candel, Coords3f({ xPos, yPos, zPos }));
	}

};


#endif // !__mm3d_entities_world_cheeriosTrack__