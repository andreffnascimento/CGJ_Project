#ifndef __RENDERER_RENDERER_H__
#define __RENDERER_RENDERER_H__


// shaderes and vertex information
#include "engine/game/scene.h"
#include "engine/renderer/vsShaderLib.h"




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
	Renderer();
	~Renderer();


public:
	void render(const Scene &scene);


private:
	GLuint _setupShaders();

};


#endif // !RENDERER_RENDERER_H
