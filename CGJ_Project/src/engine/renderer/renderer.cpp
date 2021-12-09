#include "renderer.h"

#include <iostream>

#include "engine/math/AVTmathLib.h"
#include "engine/renderer/VertexAttrDef.h"
#include "engine/scene/components/camera.h"
#include "engine/scene/components/renderable.h"
#include "engine/text/avtFreeType.h"


#include <GL/glew.h>
#include <IL/il.h>

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];




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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	// setup shaders
	if (!_setupShaders())
		throw std::string("Unable to initialize the shaders");
}


Renderer::~Renderer() 
{
	// empty
}




void Renderer::renderScene(const Scene& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (scene.getActiveCamera() != nullptr)
		scene.getActiveCamera()->renderCamera(*this);

	glUseProgram(_shader.getProgramIndex());

	float lightPos[4] = { 1.0f, 12.0f, 1.0f ,1.0f };
	float res[4];
	multMatrixPoint(VIEW, lightPos, res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(_uniformLocation[Renderer::ShaderUniformType::L_POS], 1, res);

	// render objects
	for (auto &renderableObject : scene.getEntitiesByType<Renderable>())
		renderableObject->renderObject(*this);
}


void Renderer::renderCamera(const Coords3f &cameraCoords, const Coords3f &targetCoords) const
{
	Coords3f up = { 0.0f, 1.0f, 0.0f };
	if (cameraCoords.x == 0.0f && cameraCoords.y != 0.0f && cameraCoords.z == 0.0f)
		up = { 0.0f, 0.0f, 1.0f };

	loadIdentity(VIEW);
	loadIdentity(MODEL);
	lookAt(cameraCoords.x, cameraCoords.y, cameraCoords.z,	// camera position
		   targetCoords.x, targetCoords.y, targetCoords.z,	// target position
		   up.x, up.y, up.z);								// up vector
}


void Renderer::renderObject(const MyMesh &mesh, const Renderable::TransformData &transform) const
{
	_loadMesh(mesh);				// this might require some optimization to stop loading meshes when they are already there
	_applyTransform(transform);
	_renderMesh(mesh);
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


void Renderer::_loadMesh(const MyMesh& mesh) const
{
	GLint loc;
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh.mat.ambient);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh.mat.diffuse);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh.mat.specular);
	loc = glGetUniformLocation(_shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh.mat.shininess);
}


void Renderer::_applyTransform(const Renderable::TransformData& transform) const
{
	pushMatrix(MODEL);

	Coords3f fixedPosition = {
		-transform.scale.x / 2 + transform.position.x,
		-transform.scale.y / 2 + transform.position.y,
		-transform.scale.z / 2 + transform.position.z
	};

	// avoid rotate operation if it is not needed (the other two are very common)
	if (transform.rotation.z != 0.0f)
		rotate(MODEL, transform.rotation.z, 0, 0, 1);

	if (transform.rotation.x != 0.0f)
		rotate(MODEL, transform.rotation.x, 1, 0, 0);

	if (transform.rotation.y != 0.0f)
		rotate(MODEL, transform.rotation.y, 0, 1, 0);

	translate(MODEL, fixedPosition.x, fixedPosition.y, fixedPosition.z);
	scale(MODEL, transform.scale.x, transform.scale.y, transform.scale.z);
}


void Renderer::_renderMesh(const MyMesh &mesh) const
{
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::VM], 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(_uniformLocation[Renderer::ShaderUniformType::PVM], 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(_uniformLocation[Renderer::ShaderUniformType::NORMAL], 1, GL_FALSE, mNormal3x3);

	glBindVertexArray(mesh.vao);	// render mesh

	if (!_shader.isProgramValid())
		throw std::string("Invalid shader program!");

	glDrawElements(mesh.type, mesh.numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	popMatrix(MODEL);
}