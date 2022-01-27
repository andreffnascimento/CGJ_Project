#ifndef __mm3d_entities_world_car__
#define __mm3d_entities_world_car__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/car/carHeadlightsScript.h"
#include "MicroMachines3D/scripts/car/carMovementScript.h"
#include "MicroMachines3D/scripts/collisionResolver/carCollisionResolver.h"




class Car : public SceneEntity
{

public:
	Car(Scene* scene)
		: SceneEntity(scene->createEntity("Car"))
	{
		GroupComponent& group = addComponent<GroupComponent>();

		ModelComponent& model = addComponent<ModelComponent>(*this, "src/MicroMachines3D/models/micra/micra.obj");
		TransformComponent& modelTransform = model.modelTransform();
		Transform::translate(modelTransform, Coords3f({ 0.0f, -1.4f, 0.0f }));
		Transform::scale(modelTransform, Coords3f({ CAR_SCALE, CAR_SCALE, CAR_SCALE }));
		Transform::rotate(modelTransform, Coords3f({ 0.0f, -90.0f, 0.0f }));

		_addCarHeadlight(scene, group, "frontLeft", 1.0f);
		_addCarHeadlight(scene, group, "frontRight", -1.0f);

		RigidbodyComponent& rigidbody = addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::DYNAMIC, 100.0f, 10.0f, 30.0f);
		rigidbody.setDragThreshold(20.0f);
		rigidbody.setUsesGravity(GRAVITY_ENABLED);

		AABBColliderComponent& collider = addComponent<AABBColliderComponent>(*this, ColliderIds::CAR, rigidbody, CAR_SIZE);
		collider.setCollisionResolver(std::make_shared<CarCollisionResolver>(scene, collider));
		collider.setRestitutionCocoefficient(0.3f);

		ScriptComponent& script = addComponent<ScriptComponent>();
		script.addScriptInstance(std::make_shared<CarHeadlightsScript>(scene));
		script.addScriptInstance(std::make_shared<CarMovementScript>(scene));
	}




private:
	void _addCarHeadlight(Scene* scene, GroupComponent& group, const char* headlightId, float sideMod)
	{
		Entity headlight = group.addNewEntity(scene, *this, "headlight-" + std::string(headlightId));
		Transform::translate(headlight, Coords3f({ 1.0f * sideMod, 0.7f, 3.0f }));
		Transform::rotate(headlight, Coords3f({ 20.0f, 3.0f * -sideMod, 0.0f }));
		LightComponent& lightComponent = headlight.addComponent<LightComponent>(LightComponent::LightType::SPOT, 20.0f, 0.7f);
		lightComponent.setEnabled(false);
	}

};


#endif // !__mm3d_entities_world_car__