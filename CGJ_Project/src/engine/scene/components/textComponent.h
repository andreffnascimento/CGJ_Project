#ifndef __engine_scene_components_textComponent__
#define __engine_scene_components_textComponent__


#include <string>




struct TextComponent
{
private:
	std::string _text;

public:
	TextComponent() = default;
	TextComponent(const TextComponent&) = default;
	TextComponent(const std::string& text) : _text(text) {}
	~TextComponent() = default;

	inline const std::string& text() const { return _text; }
	inline operator const std::string& () const { return text(); }

};


#endif // !__engine_scene_components_textComponent__