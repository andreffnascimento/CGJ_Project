#include "renderer.h"

#include <iostream>

#include "engine/renderer/VertexAttrDef.h"
#include "engine/renderer/Texture_Loader.h"

#include "engine/text/avtFreeType.h"

#include <GL/glew.h>
#include <IL/il.h>




void Renderer::init()
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

	// generates the texture names
	glGenTextures(Renderer::MAX_TEXTURES, _textures);

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// setup shaders
	if (!_setupShaders())
		throw std::string("Unable to initialize the shaders");
}


void Renderer::updateViewport(CameraComponent& camera, int width, int height) const
{
	if (camera.cameraProjection() == CameraComponent::CameraProjection::ORTHOGRAPHIC)
		_setOrthographicViewport(camera, width, height);
	else
		_setPerspectiveViewport(camera, width, height);
}


void Renderer::initSceneRendering() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_shader.getProgramIndex());
}




int Renderer::create2dTexture(const char* texturePath)
{
	int textureId = _nTextures++;
	Texture2D_Loader(_textures, texturePath, textureId);
	return textureId;
}


int Renderer::createCubeMapTexture(const char** texturePaths)
{
	int textureId = _nTextures++;
	TextureCubeMap_Loader(_textures, texturePaths, textureId);
	return textureId;
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

	std::cout << "InfoLog for Per Fragment Phong Lightning Shader\n" << _shader.getAllInfoLogs().c_str()  << "\n\n";

	// Shader for bitmap Text
	_shaderText.init();
	_shaderText.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/shaders/text.vert");
	_shaderText.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/shaders/text.frag");

	glLinkProgram(_shaderText.getProgramIndex());
	std::cout << "InfoLog for Text Rendering Shader\n" << _shaderText.getAllInfoLogs().c_str() << "\n\n";

	return _shader.isProgramLinked() && _shaderText.isProgramLinked();
}