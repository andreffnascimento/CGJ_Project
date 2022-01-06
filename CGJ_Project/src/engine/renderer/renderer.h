#ifndef __engine_renderer_renderer__
#define __engine_renderer_renderer__


#include "engine/renderer/vsShaderLib.h"
#include "engine/renderer/rendererData.h"
#include "engine/renderer/rendererSettings.h"

#include "engine/scene/entity.h"
#include "engine/scene/scene.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class Renderer {

private:
	VSShaderLib _shader;
	VSShaderLib _shaderText;
	int _uniformLocation[RendererData::ShaderUniformType::N_UNIFORMS] = {};

	RendererData::TextureData _textures = RendererData::TextureData();
	RendererData::meshInstances_t _solidMeshInstances = RendererData::meshInstances_t();

	RendererSettings::ReflectionCoefficients _reflectionCoefficients = RendererSettings::ReflectionCoefficients();
	RendererSettings::Fog _fog = RendererSettings::Fog();


	

public:
	static unsigned int create2dTexture(const char* texturePath);
	static unsigned int createCubeMapTexture(const char** texturePaths);


public:
	static void setReflectionCoefficients(const RendererSettings::ReflectionCoefficients& reflectionCoefficients);
	static void setFog(const RendererSettings::Fog& fog);
	static void setFogActive(bool active);



public:
	Renderer() = default;
	~Renderer() = default;


public:
	void init();
	void updateViewport(CameraComponent& camera, int width, int height) const;
	void submitRenderableMesh(const MeshComponent& mesh);
	void submitRenderableEntity(const MeshComponent& mesh, const Entity& entity);


public:
	void renderScene(const Scene& scene) const;
	void initSceneRendering() const;
	void terminateSceneRendering() const;

	void renderCamera(const Scene& scene) const;
	void renderLights(const Scene& scene) const;
	void renderMeshes(const Scene& scene) const;
	void renderColliders(const Scene& scene) const;




private:
	GLuint _setupShaders();
	void _activateTextures() const;
	void _submitFogData() const;


private:
	void _setOrthographicViewport(CameraComponent& camera, int width, int height) const;
	void _setPerspectiveViewport(CameraComponent& camera, int width, int height) const;


private:
	void _formatDirectionalLight(const LightComponent& light, const Coords3f& direction, RendererData::SubmitLightData& lightData) const;
	void _formatPointLight(const LightComponent& light, const Coords3f& translation, RendererData::SubmitLightData& lightData) const;
	void _formatSpotLight(const LightComponent& light, const Coords3f& translation, const Coords3f& direction, RendererData::SubmitLightData& lightData) const;
	void _submitLightData(const RendererData::SubmitLightData& lightData) const;


private:
	void _loadMesh(const MeshComponent& mesh) const;
	void _loadTexture(const MeshComponent& mesh) const;
	void _applyTransform(const Entity& entity) const;
	void _renderMesh(const MeshComponent& mesh) const;


private:
	const MeshComponent& getColliderMesh() const;
	void _applyColliderTransform(const AABBColliderComponent& collider) const;

};


#endif // !__engine_renderer_renderer__