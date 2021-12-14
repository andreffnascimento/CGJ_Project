#ifndef __ENGINE_SCENE_COMPONENTS_H__
#define __ENGINE_SCENE_COMPONENTS_H__


#include <string>
#include <regex>
#include <unordered_set>

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

	friend class Transform;
};




struct MeshComponent
{
private:
	MyMesh _mesh;

public:
	MeshComponent() = default;
	MeshComponent(const MeshComponent& mesh) = default;
	MeshComponent(const MyMesh& mesh) : _mesh(mesh) {}
	~MeshComponent() = default;

};



#endif // !__ENGINE_SCENE_COMPONENTS_H__