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
		IMAGE_TEXTURING = 3,	// used to represent billboards 
	};




private:
	Texture::TextureMode _textureMode = TextureMode::NONE;
	unsigned int _textureIds[RendererSettings::MAX_TEXTURES_PER_MESH] = {};
	unsigned int _normalIds[RendererSettings::MAX_TEXTURES_PER_MESH] = {};
	size_t _nTextures = 0;
	size_t _nNormals = 0;




public:
	inline const Texture::TextureMode& textureMode() const { return _textureMode; }
	inline const unsigned int* textureIds() const { return _textureIds; }
	inline const unsigned int* normalIds() const  { return _normalIds; }
	inline size_t nTextures() const { return _nTextures; }
	inline size_t nNormals() const { return _nNormals; }


public:
	inline void setTextureMode(const Texture::TextureMode& textureMode) { _textureMode = textureMode; }
	inline void addTexture(unsigned int textureId) { _textureIds[_nTextures++] = textureId; }
	inline void addNormal(unsigned int normalId) { _normalIds[_nNormals++] = normalId; }

};


#endif // !__engine_renderer_mesh_texture__