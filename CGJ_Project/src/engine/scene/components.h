#ifndef __engine_scene_components__
#define __engine_scene_components__


#include <string>
#include <regex>
#include <list>
#include <memory>
#include <unordered_set>

#include "engine/scene/entity.h"
#include "engine/scene/script.h"

#include "engine/math/transformMatrix.h"
#include "engine/physics/force.h"
#include "engine/renderer/geometry.h"
#include "engine/utils/coords.h"


class Scene;
class CollisionResolver;




struct TagComponent
{
private:
	std::string _tag;

public:
	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag) : _tag(tag) {}
	~TagComponent() = default;

	inline const std::string& tag() const		{ return _tag; }
	inline operator const std::string& () const { return tag(); }

	inline bool operator==(const TagComponent& other) const { return _tag == other._tag; }
	inline bool operator!=(const TagComponent& other) const { return _tag != other._tag; }

	inline bool operator==(const std::string& tag) const { return _tag == tag; }
	inline bool operator!=(const std::string& tag) const { return _tag != tag; }

	inline bool operator==(const std::regex& regex) const { return std::regex_match(_tag, regex); }
	inline bool operator!=(const std::regex& regex) const { return !std::regex_match(_tag, regex); }
};




struct TransformComponent
{
private:
	Coords3f _translation = { 0.0f, 0.0f, 0.0f };
	Quaternion _rotation = Quaternion();
	Coords3f _scale = { 1.0f, 1.0f, 1.0f };

	TransformMatrix _localTransform;
	TransformMatrix _worldTransform;
	bool _locallyUpdated = false;
	bool _globallyUpdated = false;

public:
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	~TransformComponent() = default;

	inline const Coords3f& translation() const	{ return _translation; }
	inline const Quaternion& rotation() const	{ return _rotation; }
	inline const Coords3f& scale() const		{ return _scale; }

	inline const TransformMatrix& transformMatrix() const { return _worldTransform; }

public:
	friend class Transform;
};




struct GroupComponent
{
private:
	std::unordered_set<Entity> _group = std::unordered_set<Entity>();

public:
	GroupComponent() = default;
	GroupComponent(const GroupComponent&) = default;
	~GroupComponent() = default;

	Entity addNewEntity(Scene* scene, const Entity& parentEntity);
	Entity addNewEntity(Scene* scene, const Entity& parentEntity, const std::string& tag);
	void expandGroup(std::unordered_set<Entity>& outExpandedGroup) const;

	inline const std::unordered_set<Entity>& group() const		{ return _group; }
	inline operator const std::unordered_set<Entity>& () const	{ return group(); }

	template <typename T>
	void expandGroupToComponent(std::unordered_set<T*>& outExpandedGroup) const
	{
		for (Entity entity : _group)
		{
			GroupComponent* groupComponent = entity.getComponentIfExists<GroupComponent>();
			if (groupComponent != nullptr)
				groupComponent->expandGroupToComponent(outExpandedGroup);

			if (entity.hasComponent<T>())
				outExpandedGroup.emplace(&entity.getComponent<T>());
		}
	}
};




struct ParentComponent
{
private:
	Entity _parent = Entity();

public:
	ParentComponent() = delete;
	ParentComponent(const ParentComponent&) = default;
	ParentComponent(const Entity& parent) : _parent(parent) {}
	~ParentComponent() = default;

	inline void setParent(const Entity& parent) { _parent = parent; }

	inline const Entity& parent() const		{ return _parent; }
	inline operator const Entity& () const	{ return parent(); }
};




struct CameraComponent
{
public:
	enum class CameraProjection
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

private:
	CameraProjection _cameraProjection = CameraProjection::ORTHOGRAPHIC;
	ClippingPlanes _clippingPlanes = { -1.0f, 1.0f };
	Coords3f _targetCoords = { 0.0f, 0.0f, 0.0f };
	Rectf _viewportRect = { -97.7f, 97.7f, -55.0f, 55.0f };
	float _fov = 60.0f;

public:
	CameraComponent() = delete;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent(const CameraComponent::CameraProjection& cameraProjection, const ClippingPlanes& clippingPlanes, float viewSize);
	~CameraComponent() = default;
	
	inline const CameraProjection& cameraProjection() const { return _cameraProjection; }
	inline const ClippingPlanes& clippingPlanes() const { return _clippingPlanes; }
	inline const Coords3f& targetCoords() const { return _targetCoords; }
	inline const Rectf& viewportRect() const { return _viewportRect; }
	inline float fov() const { return _fov; }

	inline void setTargetCoords(const Coords3f& targetCoords) { _targetCoords = targetCoords; }

private:
	void _setOrthographicCamera(float viewportSize);
	void _setPerspectiveCamera(float fov);

public:
	friend class Renderer;
};




struct MeshComponent
{
public:
	enum class TextureMode
	{
		MODULATE_DIFFUSE = 1,	// phong color is modulated with texel color
		REPLACE_DIFFUSE = 2,	// diffuse color is replaced by texel color with specular area
	};

public:
	struct Texture
	{
	public:
		static constexpr size_t MAX_TEXTURES = 2;

	private:
		MeshComponent::TextureMode _textureMode = TextureMode::MODULATE_DIFFUSE;
		unsigned int _textureIds[MAX_TEXTURES] = {};
		size_t _nTextures = 0;

	public:
		inline const MeshComponent::TextureMode& textureMode() const	{ return _textureMode; }
		inline const unsigned int* textureIds() const					{ return _textureIds; }
		inline size_t nTextures() const									{ return _nTextures; }

	public:
		friend struct MeshComponent;
	};


private:
	std::shared_ptr<MyMesh> _mesh = nullptr;
	std::shared_ptr<MeshComponent::Texture> _texture = nullptr;
	bool _enabled = true;

	
public:
	MeshComponent() = delete;
	MeshComponent(const MeshComponent&) = default;
	MeshComponent(MyMesh&& mesh, const Material& material);
	~MeshComponent() = default;

	inline const MyMesh& mesh()	const						{ return *_mesh; }
	inline const Material& material() const					{ return _mesh->mat; }
	inline const MeshComponent::Texture& texture() const	{ return *_texture; }
	inline bool enabled() const								{ return _enabled; }

	inline void setEnabled(bool enabled)								{ _enabled = enabled; }
	void setTextureMode(const MeshComponent::TextureMode& textureMode)	{ _texture->_textureMode = textureMode; }
	void addTexture(int textureId)										{ _texture->_textureIds[_texture->_nTextures++] = textureId; }

	void setMaterial(const Material& material);
};




struct ScriptComponent
{
private:
	std::list<std::shared_ptr<Script>> _scripts = std::list<std::shared_ptr<Script>>();

public:
	ScriptComponent() = default;
	ScriptComponent(const ScriptComponent&) = default;
	ScriptComponent(const std::shared_ptr<Script>& script) { _scripts.push_back(script); }
	~ScriptComponent() = default;

	void addScriptInstance(const std::shared_ptr<Script>& script) { _scripts.push_back(script); }

	void onCreate() const;
	void onUpdate(float ts) const;
};




struct LightComponent
{
public:
	enum class LightType
	{
		DIRECTIONAL = 1,
		POINT = 2,
		SPOT = 3
	};

private:
	LightComponent::LightType _lightType = LightComponent::LightType::DIRECTIONAL;
	float _intensity = 1.0f;
	float _cutOff = 0.0f;
	bool _enabled = true;

public:
	LightComponent() = delete;
	LightComponent(const LightComponent&) = default;
	LightComponent(LightComponent::LightType lightType, float intensity);
	LightComponent(LightComponent::LightType lightType, float intensity, float cutOff);
	~LightComponent() = default;

	inline const LightType& lightType() const { return _lightType; }
	inline float intensity()			const { return _intensity; }
	inline float cutOff()				const { return _cutOff; }

	inline bool isEnabled()	const			{ return _enabled; }
	inline void setEnabled(bool enabled)	{ _enabled = enabled; }
};




struct RigidbodyComponent
{
public:
	enum class RigidbodyType
	{
		STATIC,
		KINEMATIC,
		DYNAMIC
	};

private:
	RigidbodyType _type = RigidbodyComponent::RigidbodyType::DYNAMIC;
	float _mass = 1.0f;
	float _drag = 0.0f;
	float _angularDrag = 0.0f;
	float _dragThreshold = 1.0f;
	bool _usesGravity = false;

	Coords3f _position = Coords3f();
	Quaternion _rotation = Coords3f();
	Coords3f _velocity = Coords3f();
	Coords3f _angularVelocity = Coords3f();

	float _sleepThreshold = 0.1f;			// smallest velocity that makes the object go to sleep
	bool _sleeping = true;					// doesn't process the objects if no changes occur

	std::list<Force> _forces = std::list<Force>();

public:
	RigidbodyComponent() = delete;
	RigidbodyComponent(const RigidbodyComponent&) = default;
	RigidbodyComponent(RigidbodyComponent::RigidbodyType type);
	RigidbodyComponent(RigidbodyComponent::RigidbodyType type, float mass, float drag, float angularDrag);
	~RigidbodyComponent() = default;

	inline const RigidbodyComponent::RigidbodyType type() const { return _type; }
	inline float mass() const									{ return  _mass == 0.0f ? std::numeric_limits<float>::max() : 1.0f / _mass; }
	inline float invMass() const								{ return _mass; }
	inline float drag() const									{ return _drag; }
	inline float angularDrag() const							{ return _angularDrag; }
	inline float dragThreshold() const							{ return _dragThreshold; }
	inline bool usesGravity() const								{ return _usesGravity; }
	inline const Coords3f& position() const						{ return _position; }
	inline const Quaternion& rotation() const					{ return _rotation; }
	inline const Coords3f& velocity() const						{ return _velocity; }
	inline const Coords3f& angularVelocity() const				{ return _angularVelocity; }
	inline bool sleeping() const								{ return _sleeping; }

	inline void setDragThreshold(float dragThreshold)			{ _dragThreshold = dragThreshold; }
	inline void setUsesGravity(bool usesGravity)				{ _usesGravity = usesGravity; }
	inline void setSleepThreshold(float sleepThreshold)			{ _sleepThreshold = sleepThreshold; }

	inline void addRelativeForce(const Coords3f& value)		{ addRelativeForce(Force(Force::ForceType::LINEAR, value)); }
	inline void addAbsoluteForce(const Coords3f& value)		{ addAbsoluteForce(Force(Force::ForceType::LINEAR, value)); }
	inline void addAngularForce(const Coords3f& value)		{ addAbsoluteForce(Force(Force::ForceType::ANGULAR, value)); }
	
	void setVelocity(const Coords3f& velocity);
	void setAngularVelocity(const Coords3f& angularVelocity);
	void addAbsoluteForce(const Force& force);
	void addRelativeForce(const Force& force);

public:
	friend class PhysicsEngine;
};




struct AABBColliderComponent
{
private:
	Coords3f _initialSize = Coords3f();
	Coords3f _boundingBox = Coords3f();
	bool _fixedBoundingBox = false;
	float _restitutionCocoefficient = 1.0f;

	RigidbodyComponent* _rigidbody = nullptr;
	CollisionResolver* _collisionResolver = nullptr;

public:
	AABBColliderComponent() = delete;
	AABBColliderComponent(const AABBColliderComponent&) = default;
	AABBColliderComponent(RigidbodyComponent& rigidbody, const Coords3f& initialSize);
	~AABBColliderComponent();

	inline const Coords3f& boundingBox() const			{ return _boundingBox; }
	inline const RigidbodyComponent& rigidbody() const	{ return *_rigidbody; }
	inline CollisionResolver* collisionResolver() const { return _collisionResolver; }

	inline void setInitialSize(const Coords3f& initialSize)					{ _initialSize = initialSize / 2.0f; }
	inline void setFixedBoundingBox(bool fixedBoundingBox)					{ _fixedBoundingBox = fixedBoundingBox; }
	inline void setRestitutionCocoefficient(float restitutionCocoefficient)	{ _restitutionCocoefficient = restitutionCocoefficient; }

public:
	friend class PhysicsEngine;
};


#endif // !__engine_scene_components__