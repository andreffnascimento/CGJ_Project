#ifndef __engine_renderer_rendererDatatypes__
#define __engine_renderer_rendererDatatypes__


#include "engine/utils/coords.h"




class RendererSettings
{

private:
	constexpr static const char* FONT_NAME = "fonts/arial.ttf";

	constexpr static size_t MAX_LIGHTS = 32;
	constexpr static size_t MAX_TEXTURES = 20;




public:
	struct ReflectionCoefficients
	{
		float ambient = 1.0f;
		float diffuse = 1.0f;
		float specular = 1.0f;
		float darkTexture = 0.1f;
	};


	struct Fog
	{
		enum class FogMode
		{
			LINEAR = 1,
			EXP = 2,
			EXP2 = 3
		};

		Fog::FogMode mode = Fog::FogMode::LINEAR;
		Coords4f color = Coords4f();
		float density = 1.0f;
		float startDistance = 0.0f;
		float endDistance = 10.0f;
		bool active = false;
	};




public:
	friend class Renderer;
	friend class RendererData;
};


#endif // !__engine_renderer_rendererDatatypes__