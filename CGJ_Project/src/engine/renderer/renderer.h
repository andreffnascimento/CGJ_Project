#ifndef __engine_renderer_renderer__
#define __engine_renderer_renderer__


#include <vector>

#include "engine/renderer/vsShaderLib.h"
#include "engine/renderer/rendererData.h"
#include "engine/renderer/rendererSettings.h"
#include "engine/renderer/rendererUniformLocations.h"
#include "engine/renderer/mesh/meshFromAssimp.h"

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
	RendererData::SkyboxData _skybox = RendererData::SkyboxData();

	RendererData::opaqueMeshInstances_t _opaqueMeshInstances = RendererData::opaqueMeshInstances_t();
	RendererData::translucentMeshInstances_t _translucentMeshInstances = RendererData::translucentMeshInstances_t();
	RendererData::imageMeshInstances_t _imageMeshInstances = RendererData::imageMeshInstances_t();

	RendererSettings::ReflectionCoefficients _reflectionCoefficients = RendererSettings::ReflectionCoefficients();
	RendererSettings::Fog _fog = RendererSettings::Fog();
	RendererSettings::ModelTransforms _modelTransforms = RendererSettings::ModelTransforms();
	bool _enableBump = true;



	

public:
	static unsigned int create2dTexture(const char* texturePath);
	static unsigned int createCubeMapTexture(const char** texturePaths);


public:
	static void setReflectionCoefficients(const RendererSettings::ReflectionCoefficients& reflectionCoefficients);
	static void setFog(const RendererSettings::Fog& fog);
	static void setFogActive(bool active);
	static void setBumpActive(bool active);
	static void setSkybox(const Entity& skyboxEntity);

public:
	Renderer() = default;
	~Renderer() = default;


public:
	void init();
	void updateViewport(CameraComponent& camera, int width, int height) const;
	void submitRenderableObject(const MeshComponent& mesh, const Entity& entity);
	void submitRenderableImage(const ImageComponent& image, const Entity& entity);
	void renderScene(const Scene& scene);


private:
	void _setupMeshShader();
	void _setupTextShader();


private:
	void _submitFogData() const;
	void _submitTextureData() const;


private:
	void _initSceneRendering();
	void _terminateSceneRendering();
	void _renderCamera(const CameraEntity& camera) const;
	void _renderSkybox() const;
	void _renderShadows(const Scene& scene) const;
	void _renderLights(const Scene& scene) const;
	void _renderMeshes(const Scene& scene) const;
	void _renderModels(const Scene& scene) const;
	void _renderImages(const Scene& scene) const;
	void _renderColliders(const Scene& scene) const;
	void _renderFixedMirrors(const Scene& scene);	
	void _renderParticles(const Scene& scene) const;
	void _renderLensFlares(const Scene& scene) const;
	void _renderCanvas(const Scene& scene) const;
	void _renderPlanarReflections(const Scene& scene) const;


private:
	void _setOrthographicViewport(CameraComponent& camera, int width, int height) const;
	void _setPerspectiveViewport(CameraComponent& camera, int width, int height) const;


private:
	void _initSkyboxRendering() const;
	void _terminateSkyboxRendering() const;
	void _submitSkyboxMeshData() const;
	void _submitSkyboxTransform() const;
	void _submitSkyboxMeshInstance() const;


private:
	void _formatDirectionalLight(const LightComponent& light, const Coords3f& direction, RendererData::SubmitLightBuffer& lightBuffer) const;
	void _formatPointLight(const LightComponent& light, const Coords3f& translation, RendererData::SubmitLightBuffer& lightBuffer) const;
	void _formatSpotLight(const LightComponent& light, const Coords3f& translation, const Coords3f& direction, RendererData::SubmitLightBuffer& lightBuffer) const;
	void _submitLightData(const RendererData::SubmitLightBuffer& lightBuffer) const;


private:
	void _initTranslucentRendering() const;
	void _terminateTranslucentRendering() const;
	void _sortTranslucentMeshInstancesInto(const Scene& scene, RendererData::translucentMeshInstances_t& sortedTranslucentMeshInstancesOut) const;

	void _addObjectToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, const TransformComponent* transform) const;
	void _submitRenderableData(const MeshData& meshData, RendererData::SubmitInstanceBuffer& instanceBuffer) const;
	void _submitMeshData(const MeshData& meshData) const;
	void _submitInstanceBuffer(const RendererData::SubmitInstanceBuffer& instanceBuffer) const;
	void _renderMesh(const MeshData& mesh, RendererData::SubmitInstanceBuffer& instanceBuffer) const;

	void _renderOpaqueMeshInstances() const;
	void _renderTranslucentMeshInstances(const RendererData::translucentMeshInstances_t& translucentMeshInstances) const;


private:
	void _enableShapeStenciling() const;
	void _enableRenderingIntoStencil() const;
	void _disableStencilRendering() const;

private:
	void _enableReflectorPlaneStenciling() const;
	void _enableReflectionsRendering() const;
	void _disableReflectionsRendering() const;

private:
	void _enableShadowsRendering() const;
	void _disableShadowsRendering() const;

private:
	void _applyRecursiveModelTransform(const aiNode* node) const;
	void _submitModelMeshData(const MyMesh& mesh) const;
	void _submitModelMeshInstance(const MyMesh& mesh) const;
	void _recusiveModelRendering(const ModelComponent &model, const aiNode* node) const;


private:
	void _initImageRendering() const;
	void _terminateImageRendering() const;
	void _addImageToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, const TransformComponent* transform, const ImageComponent::ImageType& imageType, const Coords3f& cameraPos) const;
	void _renderImageMeshInstances(const Scene& scene) const;

private:
	const MeshComponent& getColliderMesh() const;
	void _addToColliderInstanceBuffer(RendererData::SubmitInstanceBuffer& colliderInstanceBuffer, const AABBColliderComponent& collider) const;
	void _applyColliderTransform(const AABBColliderComponent& collider) const;


private:
	void _initParticleRendering() const;
	void _terminateParticleRendering() const;
	void _submitParticleTextureData(const ParticleGeneratorComponent & particleGenerator) const;
	void _addToParticleInstanceBuffer(RendererData::SubmitInstanceBuffer & instanceBuffer, const ParticleGeneratorComponent::ParticleData particle) const;
	void _renderParticleGenerator(const ParticleGeneratorComponent & particleGenerator) const;

private:
	void _initLensFlareRendering() const;
	void _terminateLensFlareRendering() const;
	WindowCoords _calculateLensFlareProjectedPos(const LensFlareComponent& lensFlare, int viewport[4]) const;
	void _addLensFlareToInstanceBuffer(RendererData::SubmitInstanceBuffer& instanceBuffer, WindowCoords flarePosition, int width, int height) const;
	void _submitLensFlareBuffer(const Coords4f& color, unsigned int colorMapId, float scaleDistance) const;
	void _renderLensFlare(const LensFlareComponent::FlareData& lensFlareData, const WindowCoords& flareProjectedPosition, int viewport[4]) const;


private:
	void _initMirrorShape(const WindowCoords& windowSize) const;
	void _initMirrorCamera(const FlatMirrorComponent& flatMirror);
	void _terminateMirrorRendering();

	void _renderMirrorShape(const FlatMirrorComponent& flatMirror) const;
	void _renderMirrorView(const Scene& scene, const FlatMirrorComponent& flatMirror);


private:
	void _initCanvasRendering() const;
	void _terminateCanvasRendering() const;
	void _renderTextInstances(const std::unordered_map<EntityHandle, CanvasComponent>& canvasComponents) const;
	void _renderImageInstances(const std::unordered_map<EntityHandle, CanvasComponent>& canvasComponents) const;

};


#endif // !__engine_renderer_renderer__