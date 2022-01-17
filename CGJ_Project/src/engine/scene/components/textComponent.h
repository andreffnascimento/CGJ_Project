#ifndef __engine_scene_components_textComponent__
#define __engine_scene_components_textComponent__


#include <string>

#include "engine/utils/coords.h"




struct TextComponent
{
private:
	std::string _text;
	float _size = 1.0f;
	Coords3f _color = Coords3f({ 1.0f, 1.0f, 1.0f });
	bool _enabled = true;

public:
	TextComponent() = delete;
	TextComponent(const TextComponent&) = default;
	TextComponent(const std::string& text) : _text(text) {}
	~TextComponent() = default;

	inline const std::string& text() const	{ return _text; }
	inline float size() const				{ return _size; }
	inline const Coords3f& color() const	{ return _color; }
	inline bool enabled() const				{ return _enabled; }

	inline operator const std::string& () const { return text(); }

	inline void setSize(float size)				{ _size = size; }
	inline void setColor(const Coords3f& color) { _color = color; }
	inline void setEnabled(bool enabled)		{ _enabled = enabled; }

};


#endif // !__engine_scene_components_textComponent__