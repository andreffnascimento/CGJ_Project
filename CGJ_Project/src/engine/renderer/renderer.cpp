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
	if (renderer._textures.n2dTextures >= RendererSettings::MAX_2D_TEXTURES)
		throw std::string("The renderer only supports up to " + std::to_string(RendererSettings::MAX_2D_TEXTURES) + " 2d textures!");

	unsigned int textureId = (unsigned int)renderer._textures.n2dTextures++;
	Texture2D_Loader(renderer._textures.textureData, texturePath, textureId);
	return textureId;
}


unsigned int Renderer::createCubeMapTexture(const char** texturePaths)
{
	Renderer& renderer = Application::getRenderer();
	if (renderer._textures.nCubeTextures >= RendererSettings::MAX_CUBE_MAP_TEXTURES)
		throw std::string("The renderer only supports up to " + std::to_string(RendererSettings::MAX_CUBE_MAP_TEXTURES) + " cube map textures!");

	unsigned int textureId = RendererSettings::MAX_2D_TEXTURES + (unsigned int)renderer._textures.nCubeTextures++;
	TextureCubeMap_Loader(renderer._textures.textureData, texturePaths, textureId);
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


void Renderer::setBumpActive(bool active)
{
	Renderer& renderer = Application::getRenderer();
	renderer._enableBump = active;
}


void Renderer::setEnvironmentalMappingBlendingAmount(float environmentalMappingBlendingAmount)
{
	Renderer& renderer = Application::getRenderer();
	renderer._environmentalMappingBlendingAmount = environmentalMappingBlendingAmount;
}


void Renderer::setSkybox(const Entity& skyboxEntity)
{
	Renderer& renderer = Application::getRenderer();
	renderer._skybox.skybox = &skyboxEntity.getComponent<SkyboxComponent>();
	renderer._skybox.transform = &skyboxEntity.transform();
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
		throw std::string("Wrong DevIL version \n");

	ilInit();
	_setupMeshShader();
	_setupTextShader();

	// generates the texture names
	glGenTextures(RendererSettings::MAX_2D_TEXTURES + RendererSettings::MAX_CUBE_MAP_TEXTURES, _textures.textureData);

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialization of freetype library with font_name file
	freeType_init(RendererSettings::FONT_NAME);
}


void Renderer::updateViewport(CameraComponent& camera, int width, int height) const
{
	if (camera.cameraProjection() == CameraComponent::CameraProjection::ORTHOGRAPHIC)
		_setOrthographicViewport(camera, width, height);
	else
		_setPerspectiveViewport(camera, width, height);
}


void Renderer::submitRenderableObject(const MeshComponent& mesh, const Entity& entity)
{
	if (mesh.material().diffuse[3] == 1.0f)		// checks if the object is translucid
	{
		if (_opaqueMeshInstances.find(&mesh.meshData()) == _opaqueMeshInstances.end())
			_opaqueMeshInstances[&mesh.meshData()] = std::unordered_map<const MeshComponent*, const TransformComponent*>();
		_opaqueMeshInstances[&mesh.meshData()][&mesh] = &entity.transform();
	}
	else
	{
		_translucentMeshInstances[&mesh] = &entity.transform();
	}
}


void Renderer::submitRenderableImage(const ImageComponent& image, const Entity& entity)
{
	if (_imageMeshInstances.find(&image.meshData()) == _imageMeshInstances.end())
		_imageMeshInstances[&image.meshData()] = std::unordered_map<const ImageComponent*, const TransformComponent*>();
	_imageMeshInstances[&image.meshData()][&image] = &entity.transform();
}


void Renderer::renderScene(const Scene& scene)
{
	_initSceneRendering();
	_renderCamera(scene.activeCamera());
	_renderSkybox();
	_renderImages(scene);
	_renderLights(scene);
	_renderPlanarReflections(scene);
	_renderShadowPlanes(scene);
	_renderModels(scene, RendererSettings::RendererMode::MESH_RENDERER);
	_renderMeshes(scene, RendererSettings::RendererMode::MESH_RENDERER);
	_renderColliders(scene);
	_renderParticles(scene);
	_renderLensFlares(scene);
	_renderFixedMirrors(scene);
	_renderCanvas(scene);
	_terminateSceneRendering();
}




void Renderer::_setupMeshShader()
{
	_meshShader.init();
	_meshShader.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/renderer/shaders/phong_blinn.vert");
	_meshShader.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/renderer/shaders/phong_blinn.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(_meshShader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(_meshShader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(_meshShader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(_meshShader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "textureCoords");

	glLinkProgram(_meshShader.getProgramIndex());

	_uniformLocator[RendererUniformLocations::INSTANCE_PVM_MATRIX] = glGetUniformLocation(_meshShader.getProgramIndex(), "instanceData.pvmMatrix");
	_uniformLocator[RendererUniformLocations::INSTANCE_VM_MATRIX] = glGetUniformLocation(_meshShader.getProgramIndex(), "instanceData.vmMatrix");
	_uniformLocator[RendererUniformLocations::INSTANCE_NORMAL_MATRIX] = glGetUniformLocation(_meshShader.getProgramIndex(), "instanceData.normalMatrix");
	_uniformLocator[RendererUniformLocations::INSTANCE_PARTICLE_COLOR] = glGetUniformLocation(_meshShader.getProgramIndex(), "instanceData.particleColor");

	_uniformLocator[RendererUniformLocations::VIEW_MATRIX] = glGetUniformLocation(_meshShader.getProgramIndex(), "viewMatrix");
	_uniformLocator[RendererUniformLocations::SKYBOX_MODEL_MATRIX] = glGetUniformLocation(_meshShader.getProgramIndex(), "skyboxModelMatrix");

	_uniformLocator[RendererUniformLocations::MATERIAL_AMBIENT] = glGetUniformLocation(_meshShader.getProgramIndex(), "materialData.ambient");
	_uniformLocator[RendererUniformLocations::MATERIAL_DIFFUSE] = glGetUniformLocation(_meshShader.getProgramIndex(), "materialData.diffuse");
	_uniformLocator[RendererUniformLocations::MATERIAL_SPECULAR] = glGetUniformLocation(_meshShader.getProgramIndex(), "materialData.specular");
	_uniformLocator[RendererUniformLocations::MATERIAL_SHININESS] = glGetUniformLocation(_meshShader.getProgramIndex(), "materialData.shininess");
	_uniformLocator[RendererUniformLocations::MATERIAL_EMISSIVE] = glGetUniformLocation(_meshShader.getProgramIndex(), "materialData.emissive");

	_uniformLocator[RendererUniformLocations::TEXTURE_MODE] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.mode");
	_uniformLocator[RendererUniformLocations::TEXTURE_N_COLOR_MAPS] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.nColorMaps");
	_uniformLocator[RendererUniformLocations::TEXTURE_N_NORMAL_MAPS] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.nNormalMaps");
	_uniformLocator[RendererUniformLocations::TEXTURE_COLOR_MAP_IDS] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.colorMapIds");
	_uniformLocator[RendererUniformLocations::TEXTURE_NORMAL_MAP_IDS] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.normalMapIds");
	_uniformLocator[RendererUniformLocations::TEXTURE_2D_MAPS] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.maps");
	_uniformLocator[RendererUniformLocations::TEXTURE_CUBE_MAPS] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.cubeMaps");
	_uniformLocator[RendererUniformLocations::BUMP_ACTIVE] = glGetUniformLocation(_meshShader.getProgramIndex(), "textureData.bumpActive");

	_uniformLocator[RendererUniformLocations::N_LIGHTS] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.nLights");
	_uniformLocator[RendererUniformLocations::LIGHT_TYPE] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.type");
	_uniformLocator[RendererUniformLocations::LIGHT_POSITION] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.position");
	_uniformLocator[RendererUniformLocations::LIGHT_DIRECTION] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.direction");
	_uniformLocator[RendererUniformLocations::LIGHT_INTENSITY] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.intensity");
	_uniformLocator[RendererUniformLocations::LIGHT_CUTOFF] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.cutOff");
	_uniformLocator[RendererUniformLocations::LIGHT_AMBIENT] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.ambientCoefficient");
	_uniformLocator[RendererUniformLocations::LIGHT_DIFFUSE] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.diffuseCoefficient");
	_uniformLocator[RendererUniformLocations::LIGHT_SPECULAR] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.specularCoefficient");
	_uniformLocator[RendererUniformLocations::LIGHT_DARK_TEXTURE] = glGetUniformLocation(_meshShader.getProgramIndex(), "lightingData.darkTextureCoefficient");

	_uniformLocator[RendererUniformLocations::FOG_MODE] = glGetUniformLocation(_meshShader.getProgramIndex(), "fogData.mode");
	_uniformLocator[RendererUniformLocations::FOG_COLOR] = glGetUniformLocation(_meshShader.getProgramIndex(), "fogData.color");
	_uniformLocator[RendererUniformLocations::FOG_DENSITY] = glGetUniformLocation(_meshShader.getProgramIndex(), "fogData.density");
	_uniformLocator[RendererUniformLocations::FOG_START_DISTANCE] = glGetUniformLocation(_meshShader.getProgramIndex(), "fogData.startDistance");
	_uniformLocator[RendererUniformLocations::FOG_END_DISTANCE] = glGetUniformLocation(_meshShader.getProgramIndex(), "fogData.endDistance");
	_uniformLocator[RendererUniformLocations::FOG_ACTIVE] = glGetUniformLocation(_meshShader.getProgramIndex(), "fogData.isActive");

	_uniformLocator[RendererUniformLocations::LENS_FLARE_COLOR] = glGetUniformLocation(_meshShader.getProgramIndex(), "lensFlareData.color");
	_uniformLocator[RendererUniformLocations::LENS_FLARE_COLOR_MAP_ID] = glGetUniformLocation(_meshShader.getProgramIndex(), "lensFlareData.colorMapId");

	_uniformLocator[RendererUniformLocations::ENVIRONMENTAL_MAPPING_BLENDING_AMOUNT] = glGetUniformLocation(_meshShader.getProgramIndex(), "environmentalMappingData.blendingAmount");

	_uniformLocator[RendererUniformLocations::RENDER_MODE] = glGetUniformLocation(_meshShader.getProgramIndex(), "renderMode");

	std::cout << "InfoLog for Per Fragment Phong Lightning Shader\n" << _meshShader.getAllInfoLogs().c_str() << "\n\n";
	if (!_meshShader.isProgramLinked())
		throw std::string("Unable to link the mesh shader program!" + _meshShader.getAllInfoLogs());
}


void Renderer::_setupTextShader()
{
	// Shader for bitmap Text
	_textShader.init();
	_textShader.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/renderer/shaders/text.vert");
	_textShader.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/renderer/shaders/text.frag");

	glLinkProgram(_textShader.getProgramIndex());

	std::cout << "InfoLog for Text Rendering Shader\n" << _textShader.getAllInfoLogs().c_str() << "\n\n";
	if (!_textShader.isProgramLinked())
		throw std::string("Unable to link the text shader program!" + _textShader.getAllInfoLogs());
}




void Renderer::_submitFogData() const
{
	glUniform1i(_uniformLocator[RendererUniformLocations::FOG_ACTIVE], _fog.active);
	if (!_fog.active)
		return;

	float fogColor[4] = { _fog.color.x, _fog.color.y, _fog.color.z, _fog.color.w };
	glUniform1ui(_uniformLocator[RendererUniformLocations::FOG_MODE], (unsigned int)_fog.mode);
	glUniform4fv(_uniformLocator[RendererUniformLocations::FOG_COLOR], 4, fogColor);
	glUniform1f(_uniformLocator[RendererUniformLocations::FOG_START_DISTANCE], _fog.startDistance);
	glUniform1f(_uniformLocator[RendererUniformLocations::FOG_END_DISTANCE], _fog.endDistance);
	glUniform1f(_uniformLocator[RendererUniformLocations::FOG_DENSITY], _fog.density);
}


void Renderer::_submitTextureData() const
{
	int texture2dMaps[RendererSettings::MAX_2D_TEXTURES] = {};
	for (unsigned int i = 0; i < _textures.n2dTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _textures.textureData[i]);
		texture2dMaps[i] = i;
	}
	for (unsigned int i = _textures.n2dTextures; i < RendererSettings::MAX_2D_TEXTURES; i++)
		texture2dMaps[i] = i;

	int textureCubeMaps[RendererSettings::MAX_CUBE_MAP_TEXTURES] = {};
	for (unsigned int i = 0; i < _textures.nCubeTextures; i++)
	{
		unsigned int j = RendererSettings::MAX_2D_TEXTURES + i;
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _textures.textureData[j]);
		textureCubeMaps[i] = j;
	}
	for (unsigned int i = _textures.nCubeTextures; i < RendererSettings::MAX_CUBE_MAP_TEXTURES; i++)
		textureCubeMaps[i] = RendererSettings::MAX_2D_TEXTURES + i;
	
	glUniform1iv(_uniformLocator[RendererUniformLocations::TEXTURE_2D_MAPS], RendererSettings::MAX_2D_TEXTURES, texture2dMaps);
	glUniform1iv(_uniformLocator[RendererUniformLocations::TEXTURE_CUBE_MAPS], RendererSettings::MAX_CUBE_MAP_TEXTURES, textureCubeMaps);
	glUniform1i(_uniformLocator[RendererUniformLocations::BUMP_ACTIVE], _enableBump);
}


void Renderer::_submitEnvironmentalMappingData() const
{
	glUniform1f(_uniformLocator[RendererUniformLocations::ENVIRONMENTAL_MAPPING_BLENDING_AMOUNT], _environmentalMappingBlendingAmount);
}




void Renderer::_initSceneRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(_meshShader.getProgramIndex());
	_submitFogData();
	_submitTextureData();
	_submitEnvironmentalMappingData();

#ifdef _DEBUG
	if (!_meshShader.isProgramValid())
		throw std::string("Invalid mesh shader program!" + _meshShader.getAllInfoLogs());

	if (!_textShader.isProgramValid())
		throw std::string("Invalid text shader program!" + _textShader.getAllInfoLogs());
#endif
}


void Renderer::_terminateSceneRendering()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}