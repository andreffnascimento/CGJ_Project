#ifndef __engine_renderer_rendererDatatypes__
#define __engine_renderer_rendererDatatypes__


#include "engine/utils/coords.h"




class RendererSettings
{

private:
	constexpr static const char* FONT_NAME = "fonts/arial.ttf";

	constexpr static size_t MAX_LIGHTS = 32;
	constexpr static size_t MAX_TEXTURES = 20;




private:
	struct ReflectionCoefficients
	{
		float ambient = 1.0f;
		float diffuse = 1.0f;
		float specular = 1.0f;
		float darkTexture = 0.1f;
	};




public:
	friend class Renderer;
	friend class RendererData;

};


#endif // !__engine_renderer_rendererDatatypes__