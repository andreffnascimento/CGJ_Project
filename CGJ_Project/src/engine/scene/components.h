#ifndef __ENGINE_SCENE_COMPONENTS_H__
#define __ENGINE_SCENE_COMPONENTS_H__


#include <string>
#include <regex>
#include <unordered_set>

#include "engine/scene/entity.h"
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

	inline const std::string& getTag() const	{ return _tag; }
	inline operator const std::string& () const { return getTag(); }

	inline bool operator==(const TagComponent& other) const { return _tag == other._tag; }
	inline bool operator!=(const TagComponent& other) const { return _tag != other._tag; }

	inline bool operator==(const std::string& tag) const { return _tag == tag; }
	inline bool operator!=(const std::string& tag) const { return _tag != tag; }

	inline bool operator==(const std::regex& regex) const { return std::regex_match(_tag, regex); }
	inline bool operator!=(const std::regex& regex) const { return !std::regex_match(_tag, regex); }
};




struct GroupComponent
{
private:
	std::unordered_set<Entity> _group = std::unordered_set<Entity>();

public:
	GroupComponent() = default;
	GroupComponent(const GroupComponent&) = default;
	~GroupComponent() = default;

	inline void addToGroup(Entity entity)						{ _group.emplace(entity); }
	inline const std::unordered_set<Entity>& getGroup() const	{ return _group; }

	inline void operator+=(Entity entity)						{ addToGroup(entity); }
	inline operator const std::unordered_set<Entity>& () const	{ return getGroup(); }

	void expandGroup(std::unordered_set<Entity>& outExpandedGroup) const
	{
		for (Entity entity : _group)
		{
			if (entity.hasComponent<GroupComponent>())
				entity.getComponent<GroupComponent>().expandGroup(outExpandedGroup);

			outExpandedGroup.emplace(entity);
		}
	}

	template <typename T>
	void expandGroupToComponent(std::unordered_set<T*>& outExpandedGroup) const
	{
		for (Entity entity : _group)
		{
			if (entity.hasComponent<GroupComponent>())
				entity.getComponent<GroupComponent>().expandGroupToComponent(outExpandedGroup);

			if (entity.hasComponent<T>())
				outExpandedGroup.emplace(&entity.getComponent<T>());
		}
	}
};




struct TransformComponent
{
private:
	Coords3f _translation	= { 0.0f, 0.0f, 0.0f };
	Coords3f _rotation		= { 0.0f, 0.0f, 0.0f };
	Coords3f _scale			= { 1.0f, 1.0f, 1.0f };

public:
	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	~TransformComponent() = default;

	inline const Coords3f& translation() const { return _translation; }
	inline const Coords3f& rotation() const { return _rotation; }
	inline const Coords3f& scale() const { return _scale; }

	friend class Transform;
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
	
	inline void setTargetCoords(const Coords3f& targetCoords) { _targetCoords = targetCoords; }

	inline const CameraProjection& cameraProjection() const { return _cameraProjection; }
	inline const ClippingPlanes& clippingPlanes() const { return _clippingPlanes; }
	inline const Coords3f& targetCoords() const { return _targetCoords; }
	inline const Rectf& viewportRect() const { return _viewportRect; }
	inline float fov() const { return _fov; }

	void setOrthographicCamera(const ClippingPlanes& clippingPlanes, const Rectf& viewportRect) 
	{
		_cameraProjection = CameraProjection::ORTHOGRAPHIC;
		_clippingPlanes = clippingPlanes;
		_viewportRect = viewportRect;
	}

	void setPerspectiveCamera(const ClippingPlanes& clippingPlanes, float fov)
	{
		_cameraProjection = CameraProjection::PERSPECTIVE;
		_clippingPlanes = clippingPlanes;
		_fov = fov;
	}
};




struct MeshComponent
{
private:
	MyMesh _mesh;

public:
	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
	MeshComponent(const MyMesh& mesh) : _mesh(mesh) {}
	~MeshComponent() = default;
};


#endif // !__ENGINE_SCENE_COMPONENTS_H__