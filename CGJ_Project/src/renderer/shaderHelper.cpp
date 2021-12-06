#include "shaderHelper.h"



// shaders
static VSShaderLib shader;  //geometry
static VSShaderLib shaderText;  //render bitmap text


// uniform locations
static int uniformLocation[UniformType::N_UNIFORMS];



GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "src/shaders/gouraud.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/shaders/gouraud.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	//glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	uniformLocation[UniformType::PVM] = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	uniformLocation[UniformType::VM] = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	uniformLocation[UniformType::NORMAL] = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	uniformLocation[UniformType::L_POS] = glGetUniformLocation(shader.getProgramIndex(), "l_pos");

	uniformLocation[UniformType::TEXT_MAP]= glGetUniformLocation(shader.getProgramIndex(), "texmap");
	uniformLocation[UniformType::TEXT_MAP_1] = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	uniformLocation[UniformType::TEXT_MAP_2] = glGetUniformLocation(shader.getProgramIndex(), "texmap2");


	printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	// Shader for bitmap Text
	shaderText.init();
	shaderText.loadShader(VSShaderLib::VERTEX_SHADER, "src/shaders/text.vert");
	shaderText.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/shaders/text.frag");

	glLinkProgram(shaderText.getProgramIndex());
	printf("InfoLog for Text Rendering Shader\n%s\n\n", shaderText.getAllInfoLogs().c_str());

	return(shader.isProgramLinked() && shaderText.isProgramLinked());
}


int getUniformLocation(UniformType uniformType)
{
	return uniformLocation[(int)uniformType];
}


VSShaderLib getShader()
{
	return shader;
}