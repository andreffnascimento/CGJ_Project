#ifndef __engine_renderer_rendererData__
#define __engine_renderer_rendererData__


#include <unordered_map>

#include "engine/renderer/rendererSettings.h"
#include "engine/renderer/mesh/meshData.h"

#include "engine/scene/components.h"




class RendererData
{

private:
	typedef std::unordered_map<const MeshData*, std::unordered_map<const MeshComponent*, const TransformComponent*>> opaqueMeshInstances_t;
	typedef std::unordered_map<const MeshComponent*, const TransformComponent*> translucentMeshInstances_t;
	typedef std::unordered_map<const MeshData*, std::unordered_map<const ImageComponent*, const TransformComponent*>> imageMeshInstances_t;




private:
	struct TextureData
	{
		unsigned int n2dTextures = 0;
		unsigned int nCubeTextures = 0;
		unsigned int textureData[RendererSettings::MAX_2D_TEXTURES + RendererSettings::MAX_CUBE_MAP_TEXTURES] = {};
	};


	struct SkyboxData
	{
		const SkyboxComponent* skybox = nullptr;
		const TransformComponent* transform = nullptr;
	};




private:

	struct SubmitLightBuffer
	{
		unsigned int nLights = 0;
		unsigned int lightType[RendererSettings::MAX_LIGHTS] = {};
		float lightPosition[4 * RendererSettings::MAX_LIGHTS] = {};
		float lightDirection[4 * RendererSettings::MAX_LIGHTS] = {};
		float lightIntensity[RendererSettings::MAX_LIGHTS] = {};
		float lightCutOff[RendererSettings::MAX_LIGHTS] = {};

		float ambientCoefficient = 1.0f;
		float diffuseCoefficient = 1.0f;
		float specularCoefficient = 1.0f;
		float darkTextureCoefficient = 0.1f;
	};


	struct SubmitInstanceBuffer
	{
		unsigned int nInstances = 0;
		float pvmMatrix[RendererSettings::MAX_INSTANCES_PER_SUBMISSION][4][4] = {};
		float vmMatrix[RendererSettings::MAX_INSTANCES_PER_SUBMISSION][4][4] = {};
		float normalMatrix[RendererSettings::MAX_INSTANCES_PER_SUBMISSION][3][3] = {};
		float particleColor[RendererSettings::MAX_INSTANCES_PER_SUBMISSION][4] = {};

		bool usesParticles = false;
	};




public:
	friend class Renderer;
	friend class RendererData;

};


#endif // !__engine_renderer_rendererData__