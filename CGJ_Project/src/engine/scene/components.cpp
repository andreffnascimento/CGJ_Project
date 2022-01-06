#include "components.h"

#include "engine/app/application.h"
#include "engine/physics/collisionResolver.h"
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




CameraComponent::CameraComponent(const CameraComponent::CameraProjection& cameraProjection, const ClippingPlanes& clippingPlanes, float viewSize)
	: _cameraProjection(cameraProjection), _clippingPlanes(clippingPlanes)
{
	if (_cameraProjection == CameraComponent::CameraProjection::ORTHOGRAPHIC)
		_setOrthographicCamera(viewSize);
	else
		_setPerspectiveCamera(viewSize);
}


void CameraComponent::_setOrthographicCamera(float viewportSize)
{
	Application& app = Application::getInstance();
	WindowCoords windowCoords = app.getWindowSize();
	float ratio = (float)windowCoords.y / (float)windowCoords.x;

	_viewportRect = { -viewportSize, viewportSize, -viewportSize * ratio, viewportSize * ratio };
}


void CameraComponent::_setPerspectiveCamera(float fov)
{
	_fov = fov;
}




MeshComponent::MeshComponent(MyMesh&& mesh, const Material& material)
	: _mesh(std::make_shared<MyMesh>(std::forward<MyMesh>(mesh))), _texture(std::make_shared<MeshComponent::Texture>())
{
	setMaterial(material);
}


void MeshComponent::setMaterial(const Material& material)
{
	memcpy(_mesh->mat.ambient, material.ambient, 4 * sizeof(float));
	memcpy(_mesh->mat.diffuse, material.diffuse, 4 * sizeof(float));
	memcpy(_mesh->mat.specular, material.specular, 4 * sizeof(float));
	memcpy(_mesh->mat.emissive, material.emissive, 4 * sizeof(float));
	_mesh->mat.shininess = material.shininess;
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




LightComponent::LightComponent(LightComponent::LightType lightType, float intensity)
	: _lightType(lightType), _intensity(intensity)
{
	if (_intensity < 0.0f)
		throw std::string("The light intensity must be a value greater than 0!");
}

LightComponent::LightComponent(LightComponent::LightType lightType, float intensity, float cutOff)
	: _lightType(lightType), _intensity(intensity), _cutOff(cutOff)
{
	if (_intensity < 0.0f)
		throw std::string("The light intensity must be a value greater than 0!");

	if (_cutOff < 0.0f || _cutOff > 1.0f)
		throw std::string("The cutoff must be a value between 0.0f and 1.0");
}




RigidbodyComponent::RigidbodyComponent(RigidbodyComponent::RigidbodyType type)
	: _type(type)
{
	if (_type == RigidbodyComponent::RigidbodyType::STATIC)
		_mass = 0.0f;
}

RigidbodyComponent::RigidbodyComponent(RigidbodyComponent::RigidbodyType type, float mass, float drag, float angularDrag)
	: _type(type), _mass(1.0f / mass), _drag(drag), _angularDrag(angularDrag)
{
	if (_mass < 0.0f)
		throw std::string("The mass must be a value greater or equal to 0!");

	if (_drag < 0.0f)
		throw std::string("The drag must be a value greater or equal to 0!");

	if (_angularDrag < 0.0f)
		throw std::string("The angular drag must be a value greater or equal to 0!");

	if (_type == RigidbodyComponent::RigidbodyType::STATIC)
		_mass = 0.0f;
}


void RigidbodyComponent::setVelocity(const Coords3f& velocity)
{
	if (_type == RigidbodyComponent::RigidbodyType::STATIC)
		throw std::string("Unable to change the velocity of static rigidbodies");

	_velocity = velocity;
	if (_velocity != Coords3f({ 0.0f, 0.0f, 0.0f }))
		_sleeping = false;
}


void RigidbodyComponent::setAngularVelocity(const Coords3f& angularVelocity)
{
	if (_type == RigidbodyComponent::RigidbodyType::STATIC)
		throw std::string("Unable to change the velocity of static rigidbodies");

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




AABBColliderComponent::AABBColliderComponent(RigidbodyComponent& rigidbody, const Coords3f& initialSize)
	: _rigidbody(&rigidbody), _initialSize(initialSize / 2.0f)
{
	// empty
}


AABBColliderComponent::~AABBColliderComponent()
{
	if (_collisionResolver != nullptr)
		delete _collisionResolver;

	_collisionResolver = nullptr;
}