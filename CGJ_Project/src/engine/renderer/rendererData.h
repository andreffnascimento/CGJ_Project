#ifndef __engine_renderer_rendererData__
#define __engine_renderer_rendererData__


#include "engine/renderer/rendererSettings.h"




class RendererData
{

private:
	constexpr static size_t DIRECTIONAL_LIGHT_TYPE = 1;
	constexpr static size_t POINT_LIGHT_TYPE = 2;
	constexpr static size_t SPOT_LIGHT_TYPE = 3;



private:
	enum ShaderUniformType {
		PVM,
		VM,
		NORMAL,
		L_POS,

		MATERIAL_AMBIENT,
		MATERIAL_DIFFUSE,
		MATERIAL_SPECULAR,
		MATERIAL_SHININESS,
		MATERIAL_EMISSIVE,

		N_TEXTURES,
		TEXTURE_MODE,
		TEXTURE_MAPS,

		N_LIGHTS,
		LIGHT_TYPES,
		LIGHT_POSITIONS,
		LIGHT_DIRECTIONS,
		LIGHT_INTENSITIES,
		LIGHT_CUTOFFS,
		LIGHT_AMBIENT,
		LIGHT_DIFFUSE,
		LIGHT_SPECULAR,
		LIGHT_DARK_TEXTURE,

		N_UNIFORMS
	};




private:
	struct TextureData
	{
		unsigned int nTextures = 0;
		unsigned int textureData[RendererSettings::MAX_TEXTURES] = {};
		unsigned int textureType[RendererSettings::MAX_TEXTURES] = {};
	};


	struct LightData
	{
		unsigned int nLights = 0;
		unsigned int lightTypes[RendererSettings::MAX_LIGHTS] = {};
		float lightPositions[4 * RendererSettings::MAX_LIGHTS] = {};
		float lightDirections[4 * RendererSettings::MAX_LIGHTS] = {};
		float lightIntensities[RendererSettings::MAX_LIGHTS] = {};
		float lightCutOffs[RendererSettings::MAX_LIGHTS] = {};

		float ambientCoefficient = 1.0f;
		float diffuseCoefficient = 1.0f;
		float specularCoefficient = 1.0f;
		float darkTextureCoefficient = 0.1f;
	};




public:
	friend class Renderer;
	friend class RendererData;

};


#endif // !__engine_renderer_rendererData__