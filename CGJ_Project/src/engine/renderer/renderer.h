#ifndef __engine_renderer_renderer__
#define __engine_renderer_renderer__


#include "engine/renderer/geometry.h"
#include "engine/renderer/vsShaderLib.h"
#include "engine/scene/ecsRegistry.h"
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
		TEXT_MAP,
		TEXT_MAP_1,
		TEXT_MAP_2,
		N_UNIFORMS
	};




private:
	constexpr static size_t MAX_LIGHTS = 32;
	constexpr static const char* FONT_NAME = "fonts/arial.ttf";


private:
	VSShaderLib _shader;		//geometry
	VSShaderLib _shaderText;	//render bitmap text

	int _uniformLocation[ShaderUniformType::N_UNIFORMS];	// uniform locations



public:
	Renderer() = default;
	~Renderer() = default;


public:
	void init();


public:
	void updateViewport(CameraComponent& camera, int width, int height) const;

	void initSceneRendering() const;
	void renderCamera(const CameraEntity& camera) const;
	void renderLights(const Scene& scene) const;
	void renderObjects(const Scene& scene) const;


private:
	GLuint _setupShaders();


private:
	void _setOrthographicViewport(CameraComponent& camera, int width, int height) const;
	void _setPerspectiveViewport(CameraComponent& camera, int width, int height) const;


private:
	void _loadMesh(const MeshComponent& mesh) const;
	void _applyTransform(const TransformComponent& transform) const;
	void _renderMesh(const MeshComponent& mesh) const;


private:
	void _initializeLightRendering() const;
	void _formatDirectionalLight(const LightComponent& light, size_t id, size_t types[], Coords4f directions[]) const;
	void _formatPointLight(const LightComponent& light, size_t id, size_t typess[], Coords4f positions[]) const;
	void _formatSpotLight(const LightComponent& light, size_t id, size_t types[], Coords4f positions[], Coords4f directions[], float spotCutOffs[]) const;
	void _submitLightData(size_t nLights, size_t types[], Coords4f positions[], Coords4f directions[], float spotCutOffs[]) const;

};


#endif // !__engine_renderer_renderer__
