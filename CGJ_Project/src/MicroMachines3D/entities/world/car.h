#ifndef __mm3d_entities_world_car__
#define __mm3d_entities_world_car__


#include "MicroMachines3D/common/include.h"

#include "MicroMachines3D/scripts/car/carHeadlightsScript.h"
#include "MicroMachines3D/scripts/car/carMovementScript.h"




class Car : public SceneEntity
{

public:
	Car(Scene* scene)
		: SceneEntity(scene->createEntity("Car"))
	{
		GroupComponent& group = addComponent<GroupComponent>();

		_initCarBody(scene, group);
		_initCarWheels(scene, group);
		_initCarHeadlights(scene, group);

		RigidbodyComponent& rigidbody = addComponent<RigidbodyComponent>(RigidbodyComponent::RigidbodyType::DYNAMIC, 100.0f, 10.0f, 30.0f);
		rigidbody.setDragThreshold(2.0f);
		rigidbody.setUsesGravity(true);

		Coords3f carSize = { CAR_BOTTOM_SIZE.x, CAR_BOTTOM_SIZE.y + CAR_TOP_SIZE.y + CAR_FLOOR_DISTANCE - 0.2f, CAR_BOTTOM_SIZE.z + CAR_BUMPER_SIZE.z };
		AABBColliderComponent& collider = addComponent<AABBColliderComponent>(rigidbody, carSize);
		collider.setRestitutionCocoefficient(0.3f);
		
		ScriptComponent& script = addComponent<ScriptComponent>();
		script.addScriptInstance(std::make_shared<CarHeadlightsScript>(scene));
		script.addScriptInstance(std::make_shared<CarMovementScript>(scene));
	}




private:
	void _initCarBody(Scene* scene, GroupComponent& group)
	{
		MeshComponent mesh = MeshComponent(createCube(), CAR_BODY_MATERIAL);

		Entity carBottom = group.addNewEntity(scene, *this, "bottom");
		carBottom.addComponent<MeshComponent>(mesh, carBottom);
		Transform::scale(carBottom, { CAR_BOTTOM_SIZE });

		Entity carTop = group.addNewEntity(scene, *this, "top");
		carTop.addComponent<MeshComponent>(mesh, carTop);
		Transform::scale(carTop, CAR_TOP_SIZE);
		Transform::translate(carTop, { 0.0f, (CAR_BOTTOM_SIZE.y + CAR_TOP_SIZE.y) / 2.0f, -CAR_BOTTOM_SIZE.z / 10.0f });

		Entity carSpoilerSupport = group.addNewEntity(scene, *this, "spoilerSupport");
		carSpoilerSupport.addComponent<MeshComponent>(mesh, carSpoilerSupport);
		Transform::scale(carSpoilerSupport, { CAR_SPOILER_SUPPORT_SIZE.x, CAR_SPOILER_SUPPORT_SIZE.y, CAR_SPOILER_SUPPORT_SIZE.z });
		Transform::rotate(carSpoilerSupport, { -45.0f, 0.0f, 0.0f });
		Transform::translate(carSpoilerSupport, { 0.0f, (CAR_BOTTOM_SIZE.y + CAR_SPOILER_SUPPORT_SIZE.y) / 2.5f, -(CAR_BOTTOM_SIZE.z - CAR_SPOILER_SUPPORT_SIZE.z) / 2.0f });

		Entity carSpoiler = group.addNewEntity(scene, *this, "spoiler");
		carSpoiler.addComponent<MeshComponent>(mesh, carSpoiler);
		Transform::scale(carSpoiler, CAR_SPOILER_SIZE);
		Transform::translate(carSpoiler, { 0.0f, CAR_BOTTOM_SIZE.y - CAR_SPOILER_SIZE.y, CAR_BOTTOM_SIZE.z / -2.0f});

		Entity carBumper = group.addNewEntity(scene, *this, "bumper");
		carBumper.addComponent<MeshComponent>(mesh, carBumper);
		Transform::scale(carBumper, CAR_BUMPER_SIZE);
		Transform::translate(carBumper, { 0.0f, -(CAR_BOTTOM_SIZE.y - CAR_BUMPER_SIZE.y) / 2.0f, (CAR_BOTTOM_SIZE.z + CAR_BUMPER_SIZE.z) / 2.0f });
	}


	void _initCarWheels(Scene* scene, GroupComponent& group)
	{
		MeshComponent mesh = MeshComponent(createTorus(WHEEL_INNER_RADIUS, WHEEL_OUTER_RADIUS, WHEEL_RINGS, WHEEL_SIDES), CAR_WHEEL_MATERIAL);
		_initCarWheel(scene, group, mesh,  1.0f,  1.0f, "1");
		_initCarWheel(scene, group, mesh, -1.0f,  1.0f, "2");
		_initCarWheel(scene, group, mesh, -1.0f, -1.0f, "3");
		_initCarWheel(scene, group, mesh,  1.0f, -1.0f, "4");
	}


	void _initCarWheel(Scene* scene, GroupComponent& group, const MeshComponent& mesh, float xMod, float zMod, const char* wheelId)
	{
		Entity wheel = group.addNewEntity(scene, *this, "wheel" + std::string(wheelId));
		wheel.addComponent<MeshComponent>(mesh, wheel);

		Transform::rotate(wheel, { 0.0f, 0.0f, 90.0f });
		Transform::scale(wheel, WHEEL_SIZE);

		float xPos = (xMod * CAR_BOTTOM_SIZE.x / 2.0f) + (xMod * WHEEL_SIZE.x / 2.0f);
		float zPos = (zMod * CAR_BOTTOM_SIZE.z / 2.0f) - (zMod * WHEEL_SIZE.z / 0.8f);
		Transform::translate(wheel, { xPos, -CAR_FLOOR_DISTANCE, zPos });
	}


	void _initCarHeadlights(Scene* scene, GroupComponent& group)
	{
		MeshComponent frontMesh = MeshComponent(createCube(), CAR_HEADLIGHT_FRONT_OFF_MATERIAL);
		MeshComponent backMesh = MeshComponent(createCube(), CAR_HEADLIGHT_BACK_OFF_MATERIAL);

		_addCarHeadlightLamp(_initCarHeadlight(scene, group, frontMesh, 1.0f, 1.0f, "frontLeft"), -1.0f);
		_addCarHeadlightLamp(_initCarHeadlight(scene, group, frontMesh, -1.0f, 1.0f, "frontRight"), 1.0f);

		_initCarHeadlight(scene, group, backMesh,   1.0f, -1.0f, "backLeft");
		_initCarHeadlight(scene, group, backMesh,  -1.0f, -1.0f, "backRight");
	}

	Entity _initCarHeadlight(Scene* scene, GroupComponent& group, const MeshComponent& mesh, float xMod, float zMod, const char* headlightId)
	{
		Entity headlight = group.addNewEntity(scene, *this, "headlight-" + std::string(headlightId));
		headlight.addComponent<MeshComponent>(mesh, headlight);

		Transform::scale(headlight, CAR_HEADLIGHT_SIZE);
		
		float xPos = (xMod * CAR_BOTTOM_SIZE.x / 2.0f) - (xMod * CAR_HEADLIGHT_SIZE.x / 1.5f);
		float yPos = CAR_HEADLIGHT_SIZE.y / 2.0f;
		float zPos = (zMod * CAR_BOTTOM_SIZE.z / 2.0f) + (zMod * CAR_HEADLIGHT_SIZE.z / 2.0f);
		Transform::translate(headlight, { xPos, yPos, zPos });
		return headlight;
	}

	void _addCarHeadlightLamp(Entity headlight, float yMod)
	{
		Transform::translate(headlight, Coords3f({ 0.0f, 0.0f, -0.05f }));
		Transform::scale(headlight, Coords3f({ 1.0f, 1.0f, 1.3f }));
		Transform::rotate(headlight, Coords3f({ 20.0f, 10.0f * yMod, 0.0f }));
		LightComponent& lightComponent = headlight.addComponent<LightComponent>(LightComponent::LightType::SPOT, 20.0f, 0.7f);
		lightComponent.setEnabled(false);
	}
};


#endif // !__mm3d_entities_world_car__