#include "components.h"

#include "engine/app/application.h"
#include "engine/physics/physicsEngine.h"




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




void ScriptComponent::onCreate() const
{
	for (auto& script : _scripts)
		script.get()->onCreate();
}


void ScriptComponent::onUpdate(float ts) const
{
	for (auto& script : _scripts)
		script.get()->onUpdate(ts);
}




LightComponent::LightComponent(LightComponent::LightType lightType, float intensity, float cutOff)
	: _lightType(lightType), _intensity(intensity), _cutOff(cutOff)
{
	if (_intensity < 0.0f)
		throw std::string("The light intensity must be a value greater than 0!");

	if (_cutOff < 0.0f || _cutOff > 1.0f)
		throw std::string("The cutoff must be a value between 0.0f and 1.0");
}




RigidbodyComponent::RigidbodyComponent(RigidbodyComponent::RigidbodyType type, float mass, float drag, float angularDrag)
	: _type(type), _mass(mass), _drag(drag), _angularDrag(angularDrag)
{
	if (_mass < 0.0f)
		throw std::string("The mass must be a value greater or equal to 0!");

	if (_drag < 0.0f)
		throw std::string("The drag must be a value greater or equal to 0!");

	if (_angularDrag < 0.0f)
		throw std::string("The angular drag must be a value greater or equal to 0!");
}


void RigidbodyComponent::setVelocity(const Coords3f& velocity)
{
	_velocity = velocity;
	if (_velocity != Coords3f({ 0.0f, 0.0f, 0.0f }))
		_sleeping = false;
}


void RigidbodyComponent::setAngularVelocity(const Coords3f& angularVelocity)
{
	_angularVelocity = angularVelocity;
	if (_angularVelocity != Coords3f({ 0.0f, 0.0f, 0.0f }))
		_sleeping = false;
}


void RigidbodyComponent::addAbsoluteForce(const Force& force)
{
	if (_type != RigidbodyComponent::RigidbodyType::DYNAMIC)
		throw std::string("Forces can only be applied to dynamic objects!");

	if (force.value() != Coords3f({ 0.0f, 0.0f, 0.0f }))
	{
		_forces.push_back(force);
		_sleeping = false;
	}
}


void RigidbodyComponent::addRelativeForce(const Force& force)
{
	if (force.value() != Coords3f({ 0.0f, 0.0f, 0.0f }))
	{
		Coords3f forceVector = force.value();
		_rotation.rotatePoint(forceVector);
		addAbsoluteForce(Force(Force::ForceType::LINEAR, forceVector));
	}
}