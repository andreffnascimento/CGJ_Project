#ifndef RENDERER_SHADER_HELPER_H
#define RENDERER_SHADER_HELPER_H



// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// shaderes and vertex information
#include "renderer/VSShaderlib.h"
#include "renderer/VertexAttrDef.h"



enum UniformType
{
	PVM,
	VM,
	NORMAL,
	L_POS,
	TEXT_MAP,
	TEXT_MAP_1,
	TEXT_MAP_2,
	N_UNIFORMS
};



GLuint setupShaders();

int getUniformLocation(UniformType uniformType);

VSShaderLib getShader();


#endif // !RENDERER_SHADER_HELPER_H