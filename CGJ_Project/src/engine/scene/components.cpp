#include "components.h"

#include "engine/scene/scene.h"
#include "engine/app/application.h"




Entity GroupComponent::addNewEntity(Scene* scene, const Entity& parentEntity)
{
	Entity entity = scene->createEntity(parentEntity.tag(), true);
	entity.addComponent<ParentComponent>(parentEntity);
	_group.emplace(entity);
	return entity;
}

Entity GroupComponent::addNewEntity(Scene* scene, const Entity& parentEntity, const std::string& tag)
{
	Entity entity = scene->createEntity(parentEntity.tag().tag() + ":" + tag);
	entity.addComponent<ParentComponent>(parentEntity);
	_group.emplace(entity);
	return entity;
}


void GroupComponent::expandGroup(std::unordered_set<Entity>& outExpandedGroup) const
{
	for (Entity entity : _group)
	{
		GroupComponent* groupComponent = entity.getComponentIfExists<GroupComponent>();
		if (groupComponent != nullptr)
			groupComponent->expandGroup(outExpandedGroup);

		outExpandedGroup.emplace(entity);
	}
}




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




void LightComponent::setDirectionalLight(const Coords3f& direction, float intensity)
{
	_lightType = LightComponent::LightType::DIRECTIONAL;
	_direction = direction;
	_intensity = intensity;
}


void LightComponent::setPointLight(float intensity)
{
	_lightType = LightComponent::LightType::POINT;
	_intensity = intensity;
}


void LightComponent::setSpotLight(const Coords3f& direction, float intensity, float cutOff)
{
	_lightType = LightComponent::LightType::SPOT;
	_direction = direction;
	_intensity = intensity;
	_cutOff = cutOff;
}