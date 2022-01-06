#ifndef __engine_renderer_texture__
#define __engine_renderer_texture__


#include "engine/renderer/rendererSettings.h"




class Texture
{

public:
	enum class TextureMode
	{
		MODULATE_DIFFUSE = 1,	// phong color is modulated with texel color
		REPLACE_DIFFUSE = 2,	// diffuse color is replaced by texel color with specular area
	};




private:
	Texture::TextureMode _textureMode = TextureMode::MODULATE_DIFFUSE;
	unsigned int _textureIds[RendererSettings::MAX_MESH_TEXTURES] = {};
	size_t _nTextures = 0;




public:
	inline const Texture::TextureMode& textureMode() const { return _textureMode; }
	inline const unsigned int* textureIds() const { return _textureIds; }
	inline size_t nTextures() const { return _nTextures; }




public:
	friend struct MeshComponent;
};


#endif // !__engine_renderer_texture__
