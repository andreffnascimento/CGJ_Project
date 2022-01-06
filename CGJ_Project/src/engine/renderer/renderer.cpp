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
	glGenTextures(Renderer::MAX_TEXTURES, _textures.textureData);

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
	_activateTextures();
}




unsigned int Renderer::create2dTexture(const char* texturePath)
{
	if (_textures.nTextures >= Renderer::MAX_TEXTURES)
		throw std::string("The renderer only supports up to " + std::to_string(Renderer::MAX_TEXTURES) + " textures!");

	unsigned int textureId = _textures.nTextures++;
	Texture2D_Loader(_textures.textureData, texturePath, textureId);
	_textures.textureType[textureId] = GL_TEXTURE_2D;
	return textureId;
}


unsigned int Renderer::createCubeMapTexture(const char** texturePaths)
{
	if (_textures.nTextures >= Renderer::MAX_TEXTURES)
		throw std::string("The renderer only supports up to " + std::to_string(Renderer::MAX_TEXTURES) + " textures!");

	unsigned int textureId = (int)_textures.nTextures++;
	TextureCubeMap_Loader(_textures.textureData, texturePaths, textureId);
	_textures.textureType[textureId] = GL_TEXTURE_CUBE_MAP;
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

	_uniformLocation[Renderer::ShaderUniformType::PVM]		= glGetUniformLocation(_shader.getProgramIndex(), "m_pvm");
	_uniformLocation[Renderer::ShaderUniformType::VM]		= glGetUniformLocation(_shader.getProgramIndex(), "m_viewModel");
	_uniformLocation[Renderer::ShaderUniformType::NORMAL]	= glGetUniformLocation(_shader.getProgramIndex(), "m_normal");

	_uniformLocation[Renderer::ShaderUniformType::MATERIAL_AMBIENT]		= glGetUniformLocation(_shader.getProgramIndex(), "materialData.ambient");
	_uniformLocation[Renderer::ShaderUniformType::MATERIAL_DIFFUSE]		= glGetUniformLocation(_shader.getProgramIndex(), "materialData.diffuse");
	_uniformLocation[Renderer::ShaderUniformType::MATERIAL_SPECULAR]	= glGetUniformLocation(_shader.getProgramIndex(), "materialData.specular");
	_uniformLocation[Renderer::ShaderUniformType::MATERIAL_SHININESS]	= glGetUniformLocation(_shader.getProgramIndex(), "materialData.shininess");
	_uniformLocation[Renderer::ShaderUniformType::MATERIAL_EMISSIVE]	= glGetUniformLocation(_shader.getProgramIndex(), "materialData.emissive");
	_uniformLocation[Renderer::ShaderUniformType::MATERIAL_TEXCOUNT]	= glGetUniformLocation(_shader.getProgramIndex(), "materialData.texCount");

	_uniformLocation[Renderer::ShaderUniformType::TEX_MODE]		= glGetUniformLocation(_shader.getProgramIndex(), "textureData.texmode");
	_uniformLocation[Renderer::ShaderUniformType::TEX_MAP_0]	= glGetUniformLocation(_shader.getProgramIndex(), "textureData.texmap0");
	_uniformLocation[Renderer::ShaderUniformType::TEX_MAP_1]	= glGetUniformLocation(_shader.getProgramIndex(), "textureData.texmap1");
	_uniformLocation[Renderer::ShaderUniformType::TEX_MAP_2]	= glGetUniformLocation(_shader.getProgramIndex(), "textureData.texmap2");

	_uniformLocation[Renderer::ShaderUniformType::N_LIGHTS]				= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.nLights");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_TYPES]			= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.lightTypes");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_POSITIONS]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.lightPositions");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_DIRECTIONS]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.lightDirections");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_INTENSITIES]	= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.lightIntensities");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_CUTOFFS]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.lightCutOffs");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_AMBIENT]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.ambientCoefficient");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_DIFFUSE]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.diffuseCoefficient");
	_uniformLocation[Renderer::ShaderUniformType::LIGHT_SPECULAR]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.specularCoefficient");

	std::cout << "InfoLog for Per Fragment Phong Lightning Shader\n" << _shader.getAllInfoLogs().c_str()  << "\n\n";

	// Shader for bitmap Text
	_shaderText.init();
	_shaderText.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/shaders/text.vert");
	_shaderText.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/shaders/text.frag");

	glLinkProgram(_shaderText.getProgramIndex());
	std::cout << "InfoLog for Text Rendering Shader\n" << _shaderText.getAllInfoLogs().c_str() << "\n\n";

	return _shader.isProgramLinked() && _shaderText.isProgramLinked();
}


void Renderer::_activateTextures() const
{
	for (unsigned int i = 0; i < _textures.nTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(_textures.textureType[i], _textures.textureData[i]);
	}
}