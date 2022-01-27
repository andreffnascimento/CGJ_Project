#ifndef __engine_renderer_mesh_texture__
#define __engine_renderer_mesh_texture__


#include "engine/renderer/rendererSettings.h"




class Texture
{

public:
	enum class TextureMode
	{
		NONE = 0,
		MODULATE_DIFFUSE = 1,	// phong color is modulated with texel color
		REPLACE_DIFFUSE = 2,	// diffuse color is replaced by texel color with specular area
	};




private:
	Texture::TextureMode _textureMode = TextureMode::NONE;
	unsigned int _colorMapIds[RendererSettings::MAX_TEXTURES_PER_MESH] = {};
	unsigned int _normalMapIds[RendererSettings::MAX_TEXTURES_PER_MESH] = {};
	size_t _nColorMaps = 0;
	size_t _nNormalMaps = 0;




public:
	inline const Texture::TextureMode& textureMode() const	{ return _textureMode; }
	inline const unsigned int* colorMapIds() const			{ return _colorMapIds; }
	inline const unsigned int* normalMapIds() const			{ return _normalMapIds; }
	inline size_t nColorMaps() const						{ return _nColorMaps; }
	inline size_t nNormalMaps() const						{ return _nNormalMaps; }


public:
	inline void setTextureMode(const Texture::TextureMode& textureMode) { _textureMode = textureMode; }
	inline void addColorMap(unsigned int colorMapId)					{ _colorMapIds[_nColorMaps++] = colorMapId; }
	inline void addNormalMap(unsigned int normalMapId)					{ _normalMapIds[_nNormalMaps++] = normalMapId; }

};


#endif // !__engine_renderer_mesh_texture__