#ifndef __engine_scene_components__
#define __engine_scene_components__


#include <string>
#include <regex>
#include <memory>
#include <unordered_set>

#include "engine/scene/entity.h"
#include "engine/scene/script.h"
#include "engine/math/transformMatrix.h"
#include "engine/renderer/geometry.h"
#include "engine/utils/coords.h"


class Entity;




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
	Coords3f _rotation = { 0.0f, 0.0f, 0.0f };
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
	inline const Coords3f& rotation() const		{ return _rotation; }
	inline const Coords3f& scale() const		{ return _scale; }

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
	ParentComponent() = default;
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
	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
	~CameraComponent() = default;
	
	inline const CameraProjection& cameraProjection() const { return _cameraProjection; }
	inline const ClippingPlanes& clippingPlanes() const { return _clippingPlanes; }
	inline const Coords3f& targetCoords() const { return _targetCoords; }
	inline const Rectf& viewportRect() const { return _viewportRect; }
	inline float fov() const { return _fov; }

	inline void setTargetCoords(const Coords3f& targetCoords) { _targetCoords = targetCoords; }

	void setOrthographicCamera(const ClippingPlanes& clippingPlanes, float viewportSize);
	void setPerspectiveCamera(const ClippingPlanes& clippingPlanes, float fov);
};




struct MeshComponent
{
private:
	std::shared_ptr<MyMesh> _mesh;
	bool _enabled = true;

public:
	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
	MeshComponent(const std::shared_ptr<MyMesh>& mesh) : _mesh(mesh) {}
	MeshComponent(const std::shared_ptr<MyMesh>& mesh, const Material& material) : _mesh(mesh) { setMaterial(material); }
	~MeshComponent() = default;

	inline bool enabled()	const			{ return _enabled; }
	inline void setEnabled(bool enabled)	{ _enabled = enabled; }

	inline const MyMesh* meshPtr()	const { return _mesh.get(); }
	inline const MyMesh& meshData()	const { return *meshPtr(); } 

	inline void setMaterial(const Material& material) { return MeshComponent::setMaterial(*_mesh, material); }

	static void setMaterial(MyMesh& mesh, const Material& material);
};




struct ScriptComponent
{
private:
	std::shared_ptr<Script> _script;

public:
	ScriptComponent() = default;
	ScriptComponent(const ScriptComponent&) = default;
	ScriptComponent(const std::shared_ptr<Script>& script) : _script(script) {}
	~ScriptComponent() = default;

	inline Script* script()		const { return _script.get(); }
	inline operator Script* ()	const { return script(); }
};




struct LightComponent
{
public:
	enum class LightType
	{
		DIRECTIONAL,
		POINT,
		SPOT
	};

private:
	LightComponent::LightType _lightType = LightComponent::LightType::DIRECTIONAL;
	Coords3f _direction = { 0.0f, 0.0f, 0.0f };
	float _intensity = 1.0f;
	float _cutOff = 0.0f;
	bool _enabled = true;

public:
	LightComponent() = default;
	LightComponent(const LightComponent&) = default;
	~LightComponent() = default;

	inline const LightType& lightType() const { return _lightType; }
	inline const Coords3f& direction()  const { return _direction; }
	inline float intensity()			const { return _intensity; }
	inline float cutOff()				const { return _cutOff; }

	inline bool isEnabled()	const { return _enabled; }
	inline void setEnabled(bool enabled) { _enabled = enabled; }

	void setDirectionalLight(const Coords3f& direction, float intensity);
	void setPointLight(float intensity);
	void setSpotLight(const Coords3f& direction, float intensity, float cutOff);
};




struct RigidbodyComponent
{
private:
	float _mass = 1.0f;
	float _drag = 0.0f;
	bool _useGravity = false;

public:
	RigidbodyComponent() = default;
	RigidbodyComponent(const RigidbodyComponent&) = default;
	~RigidbodyComponent() = default;

	inline float mass() const		{ return _mass; }
	inline float drag() const		{ return _drag; }
	inline bool usesGravity() const { return _useGravity; }
};


#endif // !__engine_scene_components__