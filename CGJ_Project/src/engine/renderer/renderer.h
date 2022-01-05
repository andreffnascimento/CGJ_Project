#ifndef __engine_renderer_renderer__
#define __engine_renderer_renderer__


#include "engine/renderer/vsShaderLib.h"

#include "engine/scene/entity.h"
#include "engine/scene/scene.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class Renderer {

public:
	enum ShaderUniformType	{
		PVM,
		VM,
		NORMAL,
		L_POS,
		MATERIAL_AMBIENT,
		MATERIAL_DIFFUSE,
		MATERIAL_SPECULAR,
		MATERIAL_SHININESS,
		MATERIAL_EMISSIVE,
		MATERIAL_TEXCOUNT,
		TEXT_MAP_0,
		TEXT_MAP_1,
		TEXT_MAP_2,
		N_UNIFORMS
	};




private:
	constexpr static size_t MAX_LIGHTS = 32;
	constexpr static size_t DIRECTIONAL_LIGHT_TYPE = 1;
	constexpr static size_t POINT_LIGHT_TYPE = 2;
	constexpr static size_t SPOT_LIGHT_TYPE = 3;

	constexpr static size_t MAX_TEXTURES = 20;

	constexpr static const char* FONT_NAME = "fonts/arial.ttf";




private:
	struct TextureData
	{
		GLuint textureData[Renderer::MAX_TEXTURES] = {};
		GLuint textureType[Renderer::MAX_TEXTURES] = {};
		size_t nTextures = 0;
	};


	struct LightData
	{
		GLuint nLights = 0;
		GLuint lightTypes[Renderer::MAX_LIGHTS] = {};
		GLfloat lightPositions[4 * Renderer::MAX_LIGHTS] = {};
		GLfloat lightDirections[4 * Renderer::MAX_LIGHTS] = {};
		GLfloat lightIntensities[Renderer::MAX_LIGHTS] = {};
		GLfloat lightCutOffs[Renderer::MAX_LIGHTS] = {};

		GLfloat ambientCoefficient = 1.0f;
		GLfloat diffuseCoefficient = 1.0f;
		GLfloat specularCoefficient = 1.0f;
	};




private:
	int _uniformLocation[ShaderUniformType::N_UNIFORMS] = {};
	
	VSShaderLib _shader;
	VSShaderLib _shaderText;

	Renderer::TextureData _textures = Renderer::TextureData();




public:
	Renderer() = default;
	~Renderer() = default;


public:
	void init();
	void updateViewport(CameraComponent& camera, int width, int height) const;
	void initSceneRendering() const;


public:
	int create2dTexture(const char* texturePath);
	int createCubeMapTexture(const char** texturePaths);


public:
	void renderCamera(const Scene& scene) const;
	void renderLights(const Scene& scene) const;
	void renderMeshes(const Scene& scene) const;
	void renderColliders(const Scene& scene) const;




private:
	GLuint _setupShaders();
	void _activateTextures() const;


private:
	void _setOrthographicViewport(CameraComponent& camera, int width, int height) const;
	void _setPerspectiveViewport(CameraComponent& camera, int width, int height) const;


private:
	void _formatDirectionalLight(const LightComponent& light, const Coords3f& direction, Renderer::LightData& lightData) const;
	void _formatPointLight(const LightComponent& light, const Coords3f& translation, Renderer::LightData& lightData) const;
	void _formatSpotLight(const LightComponent& light, const Coords3f& translation, const Coords3f& direction, Renderer::LightData& lightData) const;
	void _submitLightData(const Renderer::LightData& lightData) const;


private:
	void _loadMesh(const MeshComponent& mesh) const;
	void _applyTransform(const Entity& entity) const;
	void _renderMesh(const MeshComponent& mesh) const;


private:
	const MeshComponent& getColliderMesh() const;
	void _applyColliderTransform(const AABBColliderComponent& collider) const;

};


#endif // !__engine_renderer_renderer__