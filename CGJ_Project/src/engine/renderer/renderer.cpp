#include "renderer.h"

#include <iostream>

#include "engine/math/AVTmathLib.h"
#include "engine/renderer/VertexAttrDef.h"
#include "engine/text/avtFreeType.h"


#include <GL/glew.h>		// include GLEW to access OpenGL 3.3 functions
#include <IL/il.h>			// DevIL library




const char* Renderer::FONT_NAME = "fonts/arial.ttf";					// font name




Renderer::Renderer() 
{
	// init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// print engine information
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	// initialization of DevIL
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		printf("wrong DevIL version \n");
		exit(0);
	}
	ilInit();

	// initialization of freetype library with font_name file
	freeType_init(Renderer::FONT_NAME);

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// setup shaders
	if (!_setupShaders())
		throw std::string("Unable to initialize the shaders");
}


Renderer::~Renderer() 
{
	// empty
}




void Renderer::render(const Scene& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);

	if (scene.getActiveCamera() != nullptr)
		scene.getActiveCamera()->render();

	glUseProgram(_shader.getProgramIndex());

	// render lights
	for (auto &lightObject : scene.getObjectsByType(GameObject::Type::LIGHT))
		lightObject->render();

	// TEMPORARY CODE TO GET SOME LIGHT
	float lightPos[4] = { 1.0f, 12.0f, 1.0f ,1.0f };
	float res[4];
	multMatrixPoint(VIEW, lightPos, res);
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::L_POS], 1, res);

	// render objects
	for (auto &renderableObject : scene.getObjectsByType(GameObject::Type::RENDERABLE))
		renderableObject->render();
}




GLuint Renderer::_setupShaders() 
{
	// Shader for models
	_shader.init();
	_shader.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/shaders/phong_blinn.vert");
	_shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/shaders/phong_blinn.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(_shader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(_shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(_shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(_shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(_shader.getProgramIndex());

	_uniformLocation[ShaderUniformType::PVM] = glGetUniformLocation(_shader.getProgramIndex(), "m_pvm");
	_uniformLocation[ShaderUniformType::VM] = glGetUniformLocation(_shader.getProgramIndex(), "m_viewModel");
	_uniformLocation[ShaderUniformType::NORMAL] = glGetUniformLocation(_shader.getProgramIndex(), "m_normal");
	_uniformLocation[ShaderUniformType::L_POS] = glGetUniformLocation(_shader.getProgramIndex(), "l_pos");

	_uniformLocation[ShaderUniformType::TEXT_MAP] = glGetUniformLocation(_shader.getProgramIndex(), "texmap");
	_uniformLocation[ShaderUniformType::TEXT_MAP_1] = glGetUniformLocation(_shader.getProgramIndex(), "texmap1");
	_uniformLocation[ShaderUniformType::TEXT_MAP_2] = glGetUniformLocation(_shader.getProgramIndex(), "texmap2");

	std::cout << "InfoLog for Per Fragment Phong Lightning Shader\n" << _shader.getAllInfoLogs().c_str()  << "\n\n";

	// Shader for bitmap Text
	_shaderText.init();
	_shaderText.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/shaders/text.vert");
	_shaderText.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/shaders/text.frag");

	glLinkProgram(_shaderText.getProgramIndex());
	std::cout << "InfoLog for Text Rendering Shader\n" << _shaderText.getAllInfoLogs().c_str() << "\n\n";

	return _shader.isProgramLinked() && _shaderText.isProgramLinked();
}