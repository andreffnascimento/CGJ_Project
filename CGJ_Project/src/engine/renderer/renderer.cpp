#include "renderer.h"

#include <iostream>

#include "engine/renderer/VertexAttrDef.h"
#include "engine/renderer/Texture_Loader.h"

#include "engine/app/application.h"
#include "engine/text/avtFreeType.h"

#include <GL/glew.h>
#include <IL/il.h>




unsigned int Renderer::create2dTexture(const char* texturePath)
{
	Renderer& renderer = Application::getRenderer();
	if (renderer._textures.nTextures >= RendererSettings::MAX_TEXTURES)
		throw std::string("The renderer only supports up to " + std::to_string(RendererSettings::MAX_TEXTURES) + " textures!");

	unsigned int textureId = (unsigned int)renderer._textures.nTextures++;
	Texture2D_Loader(renderer._textures.textureData, texturePath, textureId);
	renderer._textures.textureType[textureId] = GL_TEXTURE_2D;
	return textureId;
}


unsigned int Renderer::createCubeMapTexture(const char** texturePaths)
{
	Renderer& renderer = Application::getRenderer();
	if (renderer._textures.nTextures >= RendererSettings::MAX_TEXTURES)
		throw std::string("The renderer only supports up to " + std::to_string(RendererSettings::MAX_TEXTURES) + " textures!");

	unsigned int textureId = (unsigned int)renderer._textures.nTextures++;
	TextureCubeMap_Loader(renderer._textures.textureData, texturePaths, textureId);
	renderer._textures.textureType[textureId] = GL_TEXTURE_CUBE_MAP;
	return textureId;
}




void Renderer::setReflectionCoefficients(const RendererSettings::ReflectionCoefficients& reflectionCoefficients)
{
	if (reflectionCoefficients.ambient < 0.0f || reflectionCoefficients.ambient > 1.0f)
		throw std::string("The ambient coefficient must be a float value between 0.0f and 1.0f!");

	if (reflectionCoefficients.diffuse < 0.0f || reflectionCoefficients.diffuse > 1.0f)
		throw std::string("The diffuse coefficient must be a float value between 0.0f and 1.0f!");

	if (reflectionCoefficients.specular < 0.0f || reflectionCoefficients.specular > 1.0f)
		throw std::string("The specular coefficient must be a float value between 0.0f and 1.0f!");

	if (reflectionCoefficients.darkTexture < 0.0f || reflectionCoefficients.darkTexture > 1.0f)
		throw std::string("The dark texture coefficient must be a float value between 0.0f and 1.0f!");

	Renderer& renderer = Application::getRenderer();
	renderer._reflectionCoefficients = reflectionCoefficients;
}


void Renderer::setFog(const RendererSettings::Fog& fog)
{
	if (fog.density < 0.0f)
		throw std::string("The fog density must be a float value greater or equal to 0.0f!");

	if (fog.startDistance < 0.0f || fog.endDistance < 0.0f)
		throw std::string("The fog distances must be a float value greater or equal to 0.0f!");

	if (fog.startDistance >= fog.endDistance)
		throw std::string("The fog start distance must be smaller than its end distance!");

	Renderer& renderer = Application::getRenderer();
	renderer._fog = fog;
}


void Renderer::setFogActive(bool active)
{
	Renderer& renderer = Application::getRenderer();
	renderer._fog.active = active;
}




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
	freeType_init(RendererSettings::FONT_NAME);

	// generates the texture names
	glGenTextures(RendererSettings::MAX_TEXTURES, _textures.textureData);

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




void Renderer::renderScene(const Scene& scene) const
{
	initSceneRendering();
	renderCamera(scene);
	renderLights(scene);
	renderMeshes(scene);
	renderColliders(scene);
	terminateSceneRendering();
}


void Renderer::initSceneRendering() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_shader.getProgramIndex());
	_activateTextures();
	_submitFogData();
}


void Renderer::terminateSceneRendering() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}




GLuint Renderer::_setupShaders() 
{
	// Shader for models
	_shader.init();
	_shader.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/renderer/shaders/phong_blinn.vert");
	_shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/renderer/shaders/phong_blinn.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(_shader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(_shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(_shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(_shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(_shader.getProgramIndex());

	_uniformLocation[RendererData::ShaderUniformType::PVM]		= glGetUniformLocation(_shader.getProgramIndex(), "m_pvm");
	_uniformLocation[RendererData::ShaderUniformType::VM]		= glGetUniformLocation(_shader.getProgramIndex(), "m_viewModel");
	_uniformLocation[RendererData::ShaderUniformType::NORMAL]	= glGetUniformLocation(_shader.getProgramIndex(), "m_normal");

	_uniformLocation[RendererData::ShaderUniformType::MATERIAL_AMBIENT]		= glGetUniformLocation(_shader.getProgramIndex(), "materialData.ambient");
	_uniformLocation[RendererData::ShaderUniformType::MATERIAL_DIFFUSE]		= glGetUniformLocation(_shader.getProgramIndex(), "materialData.diffuse");
	_uniformLocation[RendererData::ShaderUniformType::MATERIAL_SPECULAR]	= glGetUniformLocation(_shader.getProgramIndex(), "materialData.specular");
	_uniformLocation[RendererData::ShaderUniformType::MATERIAL_SHININESS]	= glGetUniformLocation(_shader.getProgramIndex(), "materialData.shininess");
	_uniformLocation[RendererData::ShaderUniformType::MATERIAL_EMISSIVE]	= glGetUniformLocation(_shader.getProgramIndex(), "materialData.emissive");

	_uniformLocation[RendererData::ShaderUniformType::N_TEXTURES]	= glGetUniformLocation(_shader.getProgramIndex(), "textureData.nTextures");
	_uniformLocation[RendererData::ShaderUniformType::TEXTURE_MODE]	= glGetUniformLocation(_shader.getProgramIndex(), "textureData.mode");
	_uniformLocation[RendererData::ShaderUniformType::TEXTURE_MAPS]	= glGetUniformLocation(_shader.getProgramIndex(), "textureData.maps");

	_uniformLocation[RendererData::ShaderUniformType::N_LIGHTS]				= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.nLights");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_TYPES]			= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.types");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_POSITIONS]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.positions");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_DIRECTIONS]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.directions");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_INTENSITIES]	= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.intensities");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_CUTOFFS]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.cutOffs");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_AMBIENT]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.ambientCoefficient");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_DIFFUSE]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.diffuseCoefficient");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_SPECULAR]		= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.specularCoefficient");
	_uniformLocation[RendererData::ShaderUniformType::LIGHT_DARK_TEXTURE]	= glGetUniformLocation(_shader.getProgramIndex(), "lightingData.darkTextureCoefficient");

	_uniformLocation[RendererData::ShaderUniformType::FOG_MODE]				= glGetUniformLocation(_shader.getProgramIndex(), "fogData.mode");
	_uniformLocation[RendererData::ShaderUniformType::FOG_COLOR]			= glGetUniformLocation(_shader.getProgramIndex(), "fogData.color");
	_uniformLocation[RendererData::ShaderUniformType::FOG_DENSITY]			= glGetUniformLocation(_shader.getProgramIndex(), "fogData.density");
	_uniformLocation[RendererData::ShaderUniformType::FOG_START_DISTANCE]	= glGetUniformLocation(_shader.getProgramIndex(), "fogData.startDistance");
	_uniformLocation[RendererData::ShaderUniformType::FOG_END_DISTANCE]		= glGetUniformLocation(_shader.getProgramIndex(), "fogData.endDistance");
	_uniformLocation[RendererData::ShaderUniformType::FOG_ACTIVE]			= glGetUniformLocation(_shader.getProgramIndex(), "fogData.isActive");

	std::cout << "InfoLog for Per Fragment Phong Lightning Shader\n" << _shader.getAllInfoLogs().c_str()  << "\n\n";

	// Shader for bitmap Text
	_shaderText.init();
	_shaderText.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/renderer/shaders/text.vert");
	_shaderText.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/renderer/shaders/text.frag");

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

void Renderer::_submitFogData() const
{
	glUniform1i(_uniformLocation[RendererData::ShaderUniformType::FOG_ACTIVE], _fog.active);
	if (!_fog.active)
		return;

	float fogColor[4] = { _fog.color.x, _fog.color.y, _fog.color.z, _fog.color.w };
	glUniform1ui(_uniformLocation[RendererData::ShaderUniformType::FOG_MODE], (unsigned int)_fog.mode);
	glUniform4fv(_uniformLocation[RendererData::ShaderUniformType::FOG_COLOR], 4, fogColor);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::FOG_START_DISTANCE], _fog.startDistance);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::FOG_END_DISTANCE], _fog.endDistance);
	glUniform1f(_uniformLocation[RendererData::ShaderUniformType::FOG_DENSITY], _fog.density);

}