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




public:
	static const char* FONT_NAME;


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
	void renderCamera(const Camera& camera) const;
	void renderObjects(const ECSRegistry& registry) const;


private:
	GLuint _setupShaders();

	void _setOrthographicViewport(CameraComponent& camera, int width, int height) const;
	void _setPerspectiveViewport(CameraComponent& camera, int width, int height) const;

public:
	void _renderObject(const MeshComponent& mesh, const TransformComponent& transform) const;
	void _loadMesh(const MeshComponent& mesh) const;
	void _applyTransform(const TransformComponent& transform) const;
	void _renderMesh(const MeshComponent& mesh) const;

};


#endif // !__engine_renderer_renderer__
