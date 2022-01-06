#ifndef __engine_scene_components_tagComponent__
#define __engine_scene_components_tagComponent__


#include <regex>
#include <string>




struct TagComponent
{
private:
	std::string _tag;

public:
	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag) : _tag(tag) {}
	~TagComponent() = default;

	inline const std::string& tag() const { return _tag; }
	inline operator const std::string& () const { return tag(); }

	inline bool operator==(const TagComponent& other) const { return _tag == other._tag; }
	inline bool operator!=(const TagComponent& other) const { return _tag != other._tag; }

	inline bool operator==(const std::string& tag) const { return _tag == tag; }
	inline bool operator!=(const std::string& tag) const { return _tag != tag; }

	inline bool operator==(const std::regex& regex) const { return std::regex_match(_tag, regex); }
	inline bool operator!=(const std::regex& regex) const { return !std::regex_match(_tag, regex); }
};


#endif // !__engine_scene_components_tagComponent__