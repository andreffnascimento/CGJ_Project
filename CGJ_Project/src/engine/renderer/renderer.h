#ifndef __RENDERER_RENDERER_H__
#define __RENDERER_RENDERER_H__


#include "engine/renderer/geometry.h"
#include "engine/renderer/vsShaderLib.h"
#include "engine/scene/scene.h"
#include "engine/scene/components/renderable.h"
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


public:
	VSShaderLib _shader;		//geometry
	VSShaderLib _shaderText;	//render bitmap text

	int _uniformLocation[ShaderUniformType::N_UNIFORMS];	// uniform locations



public:
	Renderer();
	~Renderer();


public:
	void renderScene(const Scene &scene);
	void renderCamera(const Coords3f &cameraCoords, const Coords3f &targetCoords) const;
	void renderObject(const MyMesh &mesh, const Renderable::TransformData &transform) const;
	void terminateRenderObject() const;


private:
	GLuint _setupShaders();
	void _loadMesh(const MyMesh &mesh) const;
	void _applyTransform(const Renderable::TransformData& transform) const;
	void _renderMesh(const MyMesh& mesh) const;

};


#endif // !RENDERER_RENDERER_H
