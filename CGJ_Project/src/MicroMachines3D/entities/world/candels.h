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

		ModelComponent model = ModelComponent("src/MicroMachines3D/models/candel/candel.obj");
		TransformComponent& modelTransform = model.modelTransform();
		Transform::rotate(modelTransform, Coords3f({ -90.0f, 0.0f, 0.0f }));
		Transform::scale(modelTransform, Coords3f({ CANDEL_SCALE, CANDEL_SCALE, CANDEL_SCALE * 0.4f }));
		Transform::translate(modelTransform, Coords3f({ 0.0f, -0.95f, 0.0f }));

		_createCandel(scene, group, model, -1.0f, -1.0f, "topLeft");
		_createCandel(scene, group, model,  0.0f, -1.0f, "topMiddle");
		_createCandel(scene, group, model,  1.0f, -1.0f, "topRight");
		_createCandel(scene, group, model, -1.0f,  1.0f, "bottomLeft");
		_createCandel(scene, group, model,  0.0f,  1.0f, "bottomMiddle");
		_createCandel(scene, group, model,  1.0f,  1.0f, "bottomRight");
		
		_createCandelLight(scene, group, -1.0f, -1.0f, "light_topLeft");
		_createCandelLight(scene, group,  0.0f, -1.0f, "light_topMiddle");
		_createCandelLight(scene, group,  1.0f, -1.0f, "light_topRight");
		_createCandelLight(scene, group, -1.0f,  1.0f, "light_bottomLeft");
		_createCandelLight(scene, group,  0.0f,  1.0f, "light_bottomMiddle");
		_createCandelLight(scene, group,  1.0f,  1.0f, "light_bottomRight");
		
		addComponent<ScriptComponent>(std::make_shared<CandelsLightScript>(scene));
	}




private:
	void _createCandel(Scene* scene, GroupComponent& group, const ModelComponent& model, float xMod, float zMod, const char* candelId)
	{
		Entity candel = group.addNewEntity(scene, *this, candelId);
		candel.addComponent<ModelComponent>(model);

		RigidbodyComponent& rigidbody = candel.addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::STATIC);
		AABBColliderComponent& collider = candel.addComponent<AABBColliderComponent>(candel, ColliderIds::CANDEL, rigidbody, CANDEL_SIZE);
		collider.setFixedBoundingBox(true);

		float xPos = (TABLE_SIZE.x - CANDEL_SIZE.x) * xMod / 2.0f;
		float yPos = CANDEL_SIZE.y / 2.0f;
		float zPos = (TABLE_SIZE.z - CANDEL_SIZE.z) * zMod / 2.0f;

		if (zMod == 1)
			Transform::rotate(candel, Coords3f({ 0.0f, 180.0f, 0.0f }));

		Transform::scale(candel, CANDEL_SIZE);
		Transform::translate(candel, Coords3f({ xPos, yPos, zPos }));
	}


	void _createCandelLight(Scene* scene, GroupComponent& group, float xMod, float zMod, const char* candelId)
	{
		Entity candelLight = group.addNewEntity(scene, *this, candelId);

		LightComponent& light = candelLight.addComponent<LightComponent>(LightComponent::LightType::POINT, 30.0f);
		light.setEnabled(false);

		float xPos = (TABLE_SIZE.x - CANDEL_SIZE.x) * xMod / 2.0f;
		float yPos = CANDEL_SIZE.y + 2.0f;
		float zPos = (TABLE_SIZE.z - CANDEL_SIZE.z) * zMod / 2.0f;

		Transform::translate(candelLight, Coords3f({ xPos, yPos, zPos }));
	}

};


#endif // !__mm3d_entities_world_cheeriosTrack__