#include "components.h"

#include "engine/app/application.h"
#include "engine/scene/scene.h"




Entity GroupComponent::addNewEntity(Scene* scene, const Entity& parentEntity)
{
	Entity entity = scene->createEntity(parentEntity.tag(), true);
	_group.emplace(entity);
	return entity;
}

Entity GroupComponent::addNewEntity(Scene* scene, const Entity& parentEntity, const std::string& tag)
{
	Entity entity = scene->createEntity(parentEntity.tag().tag() + ":" + tag);
	_group.emplace(entity);
	return entity;
}


void GroupComponent::expandGroup(std::unordered_set<Entity>& outExpandedGroup) const
{
	for (Entity entity : _group)
	{
		if (entity.hasComponent<GroupComponent>())
			entity.getComponent<GroupComponent>().expandGroup(outExpandedGroup);

		outExpandedGroup.emplace(entity);
	}
}


#include <iostream>

void CameraComponent::setOrthographicCamera(const ClippingPlanes& clippingPlanes, float viewportSize)
{
	Application& app = Application::getInstance();
	WindowCoords windowCoords = app.getWindowSize();
	float ratio = (float)windowCoords.y / (float)windowCoords.x;

	_cameraProjection = CameraProjection::ORTHOGRAPHIC;
	_clippingPlanes = clippingPlanes;
	_viewportRect = { -viewportSize, viewportSize, -viewportSize * ratio, viewportSize * ratio };
}


void CameraComponent::setPerspectiveCamera(const ClippingPlanes& clippingPlanes, float fov)
{
	_cameraProjection = CameraProjection::PERSPECTIVE;
	_clippingPlanes = clippingPlanes;
	_fov = fov;
}




void MeshComponent::setMaterial(MyMesh& mesh, const Material& material)
{
	memcpy(mesh.mat.ambient, material.ambient, 4 * sizeof(float));
	memcpy(mesh.mat.diffuse, material.diffuse, 4 * sizeof(float));
	memcpy(mesh.mat.specular, material.specular, 4 * sizeof(float));
	memcpy(mesh.mat.emissive, material.emissive, 4 * sizeof(float));
	mesh.mat.shininess = material.shininess;
	mesh.mat.texCount = material.texCount;
}




void LightComponent::setDirectionalLight(const Coords3f& direction)
{
	_lightType = LightComponent::LightType::DIRECTIONAL;
	_direction = direction;
}


void LightComponent::setPointLight()
{
	_lightType = LightComponent::LightType::POINT;
}


void LightComponent::setSpotLight(const Coords3f& direction, float cutOff)
{
	_lightType = LightComponent::LightType::SPOT;
	_direction = direction;
	_cutOff = cutOff;
}
