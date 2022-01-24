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




MeshComponent::MeshComponent(const MeshComponent& mesh)
	: _meshData(mesh._meshData), _entity(mesh._entity)
{
	if (_entity != nullptr)
	{
		Renderer& renderer = Application::getRenderer();
		renderer.submitRenderableEntity(*this, *_entity);
	}
}


MeshComponent::MeshComponent(const MyMesh& mesh, const Material& material)
	: _meshData(std::make_shared<MeshData>(mesh, material)), _entity(nullptr)
{
	Renderer& renderer = Application::getRenderer();
	renderer.submitRenderableMesh(*this);
}


MeshComponent::MeshComponent(const MyMesh& mesh, const Material& material, const Entity& entity)
	: _meshData(std::make_shared<MeshData>(mesh, material)), _entity(&entity)
{
	// empty
}


MeshComponent::MeshComponent(const MeshComponent& mesh, const Entity& entity)
	: _meshData(mesh._meshData), _entity(&entity)
{
	// emtpty
}




void ScriptComponent::onCreate() const
{
	for (auto& script : _scripts)
		script->onCreate();
}


void ScriptComponent::onFixedUpdate(float ts) const
{
	for (auto& script : _scripts)
		script->onFixedUpdate(ts);
}


void ScriptComponent::onUpdate(float ts) const
{
	for (auto& script : _scripts)
		script->onUpdate(ts);
}


Script* ScriptComponent::getScriptByTag(const std::string& tag)
{
	for (auto& script : _scripts)
		if (*script == tag)
			return script.get();

	return nullptr;
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

AABBColliderComponent::AABBColliderComponent(unsigned int id, RigidbodyComponent& rigidbody, const Coords3f& initialSize)
	: _id(id), _rigidbody(&rigidbody), _initialSize(initialSize / 2.0f)
{
	// empty
}


AABBColliderComponent::~AABBColliderComponent()
{
	if (_collisionResolver != nullptr)
		delete _collisionResolver;

	_collisionResolver = nullptr;
}




Entity CanvasComponent::createTextEntity(Scene* scene, const Entity& canvasEntity, const std::string& tag)
{
	Entity entity = scene->createEntity(canvasEntity.tag().tag() + ":" + tag);
	TextComponent& textComponent = entity.addComponent<TextComponent>();
	_canvasText[&textComponent] = &entity.transform();
	return entity;
}

Entity CanvasComponent::createTextEntity(Scene* scene, const Entity& canvasEntity)
{
	Entity entity = scene->createEntity(canvasEntity.tag(), true);
	TextComponent& textComponent = entity.addComponent<TextComponent>();
	_canvasText[&textComponent] = &entity.transform();
	return entity;
}


Entity CanvasComponent::createImageEntity(Scene* scene, const Entity& canvasEntity, const std::string& tag)
{
	Entity entity = scene->createEntity(canvasEntity.tag().tag() + ":" + tag);
	ImageComponent& imageComponent = entity.addComponent<ImageComponent>();
	_canvasImage[&imageComponent] = &entity.transform();
	return entity;
}


Entity CanvasComponent::createImageEntity(Scene* scene, const Entity& canvasEntity)
{
	Entity entity = scene->createEntity(canvasEntity.tag(), true);
	ImageComponent& imageComponent = entity.addComponent<ImageComponent>();
	_canvasImage[&imageComponent] = &entity.transform();
	return entity;
}




ImageComponent::ImageComponent()
{
	Material material = {
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		0.0f,
	};

	_meshData = std::make_shared<MeshData>(createQuad(1.0f, 1.0f), material);
}


void ImageComponent::setBlendColor(const Coords4f& blendColor)
{
	Material material = _meshData->material();
	material.ambient[0] = blendColor.x;
	material.ambient[1] = blendColor.y;
	material.ambient[2] = blendColor.z;
	material.ambient[3] = blendColor.w;
	_meshData->setMaterial(material);
}




ParticleGeneratorComponent::ParticleGeneratorComponent(unsigned int textureId)
{
	_meshData.setTextureMode(Texture::TextureMode::MODULATE_DIFFUSE);
	_meshData.addTexture(textureId);
}


ParticleGeneratorComponent::~ParticleGeneratorComponent()
{
	delete _particles;
	_particles = nullptr;
}


void ParticleGeneratorComponent::initializeParticleStorage(unsigned int nParticles)
{
	_nParticles = nParticles;
	if (_nParticles > RendererSettings::MAX_PARTICLES_PER_GENERATOR)
		throw std::string("Renderer only suports up to " + std::to_string(RendererSettings::MAX_PARTICLES_PER_GENERATOR) + " particles per generator!");
	_particles = new ParticleGeneratorComponent::ParticleData[_nParticles];
}