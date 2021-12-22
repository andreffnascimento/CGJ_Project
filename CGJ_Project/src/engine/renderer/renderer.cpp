#include "renderer.h"

#include <iostream>
#include <unordered_map>

#include "engine/math/AVTmathLib.h"
#include "engine/renderer/VertexAttrDef.h"
#include "engine/text/avtFreeType.h"


#include <GL/glew.h>
#include <IL/il.h>


extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




const char* Renderer::FONT_NAME = "fonts/arial.ttf";					// font name




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

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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


void Renderer::renderCamera(const CameraEntity& camera) const
{
	TransformComponent& transform = camera.getComponent<TransformComponent>();
	CameraComponent& cameraSettings = camera.getComponent<CameraComponent>();

	const Coords3f& cameraCoords = transform.translation();
	const Coords3f& targetCoords = cameraSettings.targetCoords();

	Coords3f up = { 0.0f, 1.0f, 0.0f };
	if (cameraCoords.x == 0.0f && cameraCoords.y != 0.0f && cameraCoords.z == 0.0f)
		up = { 0.0f, 0.0f, 1.0f };

	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(cameraCoords.x, cameraCoords.y, cameraCoords.z,	// camera position
		   targetCoords.x, targetCoords.y, targetCoords.z,	// target position
		   up.x, up.y, up.z);								// up vector
}


void Renderer::renderObjects(const ECSRegistry& registry) const
{
	// temp light code
	float lightPos[4] = { -30.0f, 20.0f, -15.0f ,1.0f };
	float res[4];
	multMatrixPoint(VIEW, lightPos, res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::L_POS], 1, res);
	// temp light code end

	std::unordered_map<EntityHandle, MeshComponent>& meshComponents = registry.getComponents<MeshComponent>();
	for (auto& iterator : meshComponents)
	{
		const MeshComponent& mesh = iterator.second;
		const TransformComponent& transform = registry.getComponent<TransformComponent>(iterator.first);
		_loadMesh(mesh);				// this might require some optimization to stop loading meshes when they are already there
		_applyTransform(transform);
		_renderMesh(mesh);
	}
}




GLuint Renderer::_setupShaders() 
{
	// Shader for models
	_shader.init();
	_shader.loadShader(VSShaderLib::VERTEX_SHADER, "src/engine/shaders/gouraud.vert");
	_shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "src/engine/shaders/gouraud.frag");

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



void Renderer::_setOrthographicViewport(CameraComponent& camera, int width, int height) const
{
	camera.setOrthographicCamera(camera.clippingPlanes(), camera.viewportRect().right);

	float left = camera.viewportRect().left;
	float right = camera.viewportRect().right;
	float bottom = camera.viewportRect().bottom;
	float top = camera.viewportRect().top;
	float near = camera.clippingPlanes().near;
	float far = camera.clippingPlanes().far;

	glViewport(0, 0, width, height);	// set the viewport to be the entire window

	// set the projection matrix
	loadIdentity(PROJECTION);
	ortho(left, right, bottom, top, near, far);
}


void Renderer::_setPerspectiveViewport(CameraComponent& camera, int width, int height) const
{
	if (height == 0)					// prevent a divide by zero, when window is too small
		height = 1;

	glViewport(0, 0, width, height);	// set the viewport to be the entire window

	// set the projection matrix
	float ratio = (float)width / (float)height;
	loadIdentity(PROJECTION);
	perspective(camera.fov(), ratio, camera.clippingPlanes().near, camera.clippingPlanes().far);
}



void Renderer::_loadMesh(const MeshComponent& mesh) const
{
	const MyMesh& meshData = mesh.getMeshData();
	GLint loc;
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, meshData.mat.ambient);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, meshData.mat.diffuse);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, meshData.mat.specular);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, meshData.mat.shininess);
}


void Renderer::_applyTransform(const TransformComponent& transform) const
{
	pushMatrix(MODEL);
	translate(MODEL, transform.translation().x, transform.translation().y, transform.translation().z);
	
	// avoid rotate operation if it is not needed (the other two are very common)
	if (transform.rotation().x != 0.0f)
		rotate(MODEL, transform.rotation().x, 1, 0, 0);

	if (transform.rotation().y != 0.0f)
		rotate(MODEL, transform.rotation().y, 0, 1, 0);

	if (transform.rotation().z != 0.0f)
		rotate(MODEL, transform.rotation().z, 0, 0, 1);

	scale(MODEL, transform.scale().x, transform.scale().y, transform.scale().z);
}


void Renderer::_renderMesh(const MeshComponent& mesh) const
{
	const MyMesh& meshData = mesh.getMeshData();

	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::VM], 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::PVM], 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(_uniformLocation[Renderer::ShaderUniformType::NORMAL], 1, GL_FALSE, mNormal3x3);

	glBindVertexArray(meshData.vao);	// render mesh

	if (!_shader.isProgramValid())
		throw std::string("Invalid shader program!");

	glDrawElements(meshData.type, meshData.numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	popMatrix(MODEL);
}