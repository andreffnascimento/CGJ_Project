#ifndef __engine_renderer_renderer__
#define __engine_renderer_renderer__


#include "engine/renderer/vsShaderLib.h"
#include "engine/renderer/rendererData.h"
#include "engine/renderer/rendererSettings.h"
#include "engine/renderer/rendererUniformLocations.h"

#include "engine/scene/entity.h"
#include "engine/scene/scene.h"
#include "engine/scene/components.h"
#include "engine/utils/coords.h"




class Renderer {

private:
	VSShaderLib _meshShader;
	VSShaderLib _textShader;

	unsigned int _uniformLocator[RendererUniformLocations::N_UNIFORMS] = {};

	RendererData::TextureData _textures = RendererData::TextureData();
	RendererData::opaqueMeshInstances_t _opaqueMeshInstances = RendererData::opaqueMeshInstances_t();
	RendererData::translucentMeshInstances_t _translucentMeshInstances = RendererData::translucentMeshInstances_t();

	RendererSettings::ReflectionCoefficients _reflectionCoefficients = RendererSettings::ReflectionCoefficients();
	RendererSettings::Fog _fog = RendererSettings::Fog();
	bool _enableBump = true;


	

public:
	static unsigned int create2dTexture(const char* texturePath);
	static unsigned int createCubeMapTexture(const char** texturePaths);


public:
	static void setReflectionCoefficients(const RendererSettings::ReflectionCoefficients& reflectionCoefficients);
	static void setFog(const RendererSettings::Fog& fog);
	static void setFogActive(bool active);
	static void setBumpActive(bool active);


public:
	Renderer() = default;
	~Renderer() = default;


public:
	void init();
	void updateViewport(CameraComponent& camera, int width, int height) const;
	void submitRenderableMesh(const MeshComponent& mesh);
	void submitRenderableEntity(const MeshComponent& mesh, const Entity& entity);


public:
	void renderScene(const Scene& scene);
	void initSceneRendering();
	void terminateSceneRendering();
	
	void renderCanvas(const Scene& scene) const;
	void renderCamera(const Scene& scene) const;
	void renderLights(const Scene& scene) const;
	void renderMeshes(const Scene& scene) const;
	void renderColliders(const Scene& scene) const;




private:
	void _setupMeshShader();
	void _setupTextShader();


private:
	void _submitFogData() const;
	void _submitTextureData() const;




private:
	void _setOrthographicViewport(CameraComponent& camera, int width, int height) const;
	void _setPerspectiveViewport(CameraComponent& camera, int width, int height) const;


private:
	void _formatDirectionalLight(const LightComponent& light, const Coords3f& direction, RendererData::SubmitLightBuffer& lightBuffer) const;
	void _formatPointLight(const LightComponent& light, const Coords3f& translation, RendererData::SubmitLightBuffer& lightBuffer) const;
	void _formatSpotLight(const LightComponent& light, const Coords3f& translation, const Coords3f& direction, RendererData::SubmitLightBuffer& lightBuffer) const;
	void _submitLightData(const RendererData::SubmitLightBuffer& lightBuffer) const;


private:
	void _enableTranslucentRendering() const;
	void _disableTranslucentRendering() const;

	void _sortTranslucentMeshInstancesInto(const Scene& scene, RendererData::translucentMeshInstances_t& sortedTranslucentMeshInstancesOut) const;

	void _renderOpaqueMeshInstances() const;
	void _renderTranslucentMeshInstances(const RendererData::translucentMeshInstances_t& translucentMeshInstances) const;

	void _addToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, const TransformComponent* transform) const;
	void _applyTransform(const TransformComponent& transform) const;

	void _submitRenderableData(const MeshData& meshData, RendererData::SubmitInstanceBuffer& instanceBuffer) const;
	void _submitMeshData(const MeshData& meshData) const;
	void _submitInstanceBuffer(const RendererData::SubmitInstanceBuffer& instanceBuffer) const;
	void _renderMesh(const MeshData& mesh, RendererData::SubmitInstanceBuffer& instanceBuffer) const;


private:
	const MeshComponent& getColliderMesh() const;
	void _addToColliderInstanceBuffer(RendererData::SubmitInstanceBuffer& colliderInstanceBuffer, const AABBColliderComponent& collider) const;
	void _applyColliderTransform(const AABBColliderComponent& collider) const;


private:
	void _initCanvasRendering() const;
	void _terminateCanvasRendering() const;
	void _renderTextInstances(const std::unordered_map<EntityHandle, CanvasComponent>&canvasComponents) const;
	void _renderImageInstances(const std::unordered_map<EntityHandle, CanvasComponent>&canvasComponents) const;

};


#endif // !__engine_renderer_renderer__