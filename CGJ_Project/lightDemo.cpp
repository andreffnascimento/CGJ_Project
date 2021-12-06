//
// CGJ: Phong Shading and Text rendered with FreeType library
// The text rendering was based on https://learnopengl.com/In-Practice/Text-Rendering
// This demo was built for learning purposes only.
// Some code could be severely optimised, but I tried to
// keep as simple and clear as possible.
//
// The code comes with no warranties, use it at your own risk.
// You may use it, or parts of it, wherever you want.
// 
// Author: João Madeiras Pereira
//

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>


// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

#include <IL/il.h>


// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "geometry.h"

#include "avtFreeType.h"

using namespace std;

#define CAPTION "CGJ Demo: Phong Shading and Text rendered with FreeType"
int WindowHandle = 0;
int WinX = 1024, WinY = 768;

unsigned int FrameCount = 0;

int nObjects = 0;

//shaders
VSShaderLib shader;  //geometry
VSShaderLib shaderText;  //render bitmap text

//File with the font
const string font_name = "fonts/arial.ttf";

/* -------- Scene Objects -------- */
//Vector with meshes
vector<struct MyMesh> meshes;

/* ---------- Mesh IDs ----------- */
int car_id = 0;
int wheel_id = 0;
int table_id = 0;
int candle_id = 0;

/* ------ Object Constants ------  */
float table_length = 100.0f;
float table_width = 40.0f;
float table_thickness = 2.5f;

float leg_length = 25.0f;
float leg_thickness = 3.0f;

float car_bodyL = 5.0f;
float car_bodyW = 2.0f;
float car_bodyT = 1.0F;

float radius = 0.45f;

/* ------------ Candles ------------ */
const float CANDLE_HEIGHT = 2;
const float CANDLE_RADIUS = 0.5f;
const int NUM_CANDLES = 6;

typedef struct Candle {
	int id;
	float x = 0.0f, y = CANDLE_HEIGHT / 2 + table_thickness, z = 0.0f;
} Candle;

Candle Candles[NUM_CANDLES];

/* --------- Car Movement -------- */
int carX = 5;
int carY = table_thickness + radius * 2;
int carZ = 5;

/* ------------ Lights ----------- */
float lightPos[4] = { 1.0f, 12.0f, 1.0f ,1.0f };
int globalLight = 1;  // 1 = lights ON,  0 = lights OFF
int pointLights = 1;
int carLights = 1;

//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;

GLint tex_loc, tex_loc1, tex_loc2;
	
// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];


void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
	glutTimerFunc(1000.0 / 60.0, refresh, 0);
	glutPostRedisplay();
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if(h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	perspective(53.13f, ratio, 0.1f, 1000.0f);
}

// ------------------------------------------------------------
//
// Light Functions
//

void addDirectionalLight(int id, float l_dirX, float l_dirY, float l_dirZ) {
	GLint loc;

	float globalLightPos[4] = {l_dirX, l_dirY, l_dirZ, 0.0};
	float res[4];
	string addressPart1 = "Lights[";
	string strId = to_string(id);
	string strIsEnabled = "].isEnabled";
	string strLightType = "].lightType";
	string strLightDir = "].lightDir";
	string address;
	multMatrixPoint(VIEW, globalLightPos, res);
	glUniform4fv(lPos_uniformId, 1, res);
	address = addressPart1 + strId + strIsEnabled;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform1i(loc, globalLight);
	address = addressPart1 + strId + strLightType;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform1i(loc, 1);
	address = addressPart1 + strId + strLightDir;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform4fv(loc, 1, res);
}

void addPointLight(int id, float l_posX, float l_posY, float l_posZ) {

	GLint loc;
	string addressPart1 = "Lights[";
	string strId = to_string(id);
	string strIsEnabled = "].isEnabled";
	string strLightType = "].lightType";
	string strPosition = "].position";
	string address;

	float pointLightPos[4] = {l_posX, l_posY, l_posZ, 1.0};
	float resPos[4];
	multMatrixPoint(VIEW, pointLightPos, resPos);
	glUniform4fv(lPos_uniformId, 1, resPos);
	address = addressPart1 + strId + strIsEnabled;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform1i(loc, pointLights);
	address = addressPart1 + strId + strLightType;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform1i(loc, 2);
	address = addressPart1 + strId + strPosition;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform4fv(loc, 1, resPos);
}

void addSpotLight(int id, float anglecutOff, float l_posX, float l_posY, float l_posZ, float l_dirX, float l_dirY, float l_dirZ) {

	GLint loc;

	string addressPart1 = "Lights[";
	string strId = to_string(id);
	string strIsEnabled = "].isEnabled";
	string strLightType = "].lightType";
	string strConeDirection = "].coneDirection";
	string strSpotCutOff = "].spotCutOff";
	string strPosition = "].position";
	string address;
	float spotLightDir[4] = {l_dirX, l_dirY, l_dirZ, 0.0};
	float spotLightPos[4] = {l_posX, l_posY, l_posZ, 1.0};
	float resDir[4];
	float resPos[4];
	multMatrixPoint(VIEW, spotLightDir, resDir);
	multMatrixPoint(VIEW, spotLightPos, resPos);
	glUniform4fv(lPos_uniformId, 1, resPos);
	address = addressPart1 + strId + strIsEnabled;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform1i(loc, carLights);
	address = addressPart1 + strId + strLightType;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform1i(loc, 3);
	address = addressPart1 + strId + strConeDirection;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform4fv(loc, 1, resDir);
	address = addressPart1 + strId + strSpotCutOff;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform1f(loc, anglecutOff);
	address = addressPart1 + strId + strPosition;
	loc = glGetUniformLocation(shader.getProgramIndex(), address.c_str());
	glUniform4fv(loc, 1, resPos);
}

// ------------------------------------------------------------
//
// Render stufff
//

void processObject(MyMesh obj) {
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(obj.vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(obj.type, obj.numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);

}

void renderLegs(float x, float y, float z) {
	pushMatrix(MODEL);

	translate(MODEL, x, y, z);
	scale(MODEL, leg_thickness, leg_length, leg_thickness);
}

void renderTable() {
	GLint loc;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, meshes[table_id].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, meshes[table_id].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, meshes[table_id].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, meshes[table_id].mat.shininess);

	//Cover
	pushMatrix(MODEL);
	scale(MODEL, table_width, table_thickness, table_length);

	processObject(meshes[table_id]);

	//Legs
	renderLegs(0, -leg_length, 0);
	processObject(meshes[table_id]);

	renderLegs(0, -leg_length, table_length - leg_thickness);
	processObject(meshes[table_id]);

	renderLegs(table_width - leg_thickness, -leg_length, table_length - leg_thickness);
	processObject(meshes[table_id]);

	renderLegs(table_width - leg_thickness, -leg_length, 0);
	processObject(meshes[table_id]);
}

void renderWheels(float x, float y, float z) {
	pushMatrix(MODEL);

	translate(MODEL, x, y, z);
	rotate(MODEL, 90.0f, 0, 0, 1);
	scale(MODEL, 0.5f, 1.0f, 0.5f);

}

void renderCar() {
	GLint loc;

	
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, meshes[car_id].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, meshes[car_id].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, meshes[car_id].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, meshes[car_id].mat.shininess);

	pushMatrix(MODEL);

	translate(MODEL, carX, carY, carZ);
	scale(MODEL, car_bodyW, car_bodyT, car_bodyL);

	processObject(meshes[table_id]);

	pushMatrix(MODEL);

	translate(MODEL, carX, carY + car_bodyT, carZ + car_bodyW/2);
	scale(MODEL, car_bodyW, car_bodyT/2, car_bodyL/2.5);

	processObject(meshes[table_id]);


	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, meshes[wheel_id].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, meshes[wheel_id].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, meshes[wheel_id].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, meshes[wheel_id].mat.shininess);


	renderWheels(carX, carY, carZ + 1);
	processObject(meshes[wheel_id]);

	renderWheels(carX + car_bodyW, carY, carZ + 1);
	processObject(meshes[wheel_id]);

	renderWheels(carX + car_bodyW, carY, carZ + car_bodyL - 1);
	processObject(meshes[wheel_id]);

	renderWheels(carX, carY, carZ + car_bodyL - 1);
	processObject(meshes[wheel_id]);
}

void renderCandles() {
	int i;
	GLint loc;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, meshes[candle_id].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, meshes[candle_id].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, meshes[candle_id].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, meshes[candle_id].mat.shininess);

	for (i = 0; i < NUM_CANDLES; i++) {
		addPointLight(Candles[i].id, Candles[i].x, Candles[i].y, Candles[i].z);

		pushMatrix(MODEL);
		translate(MODEL, Candles[i].x, Candles[i].y, Candles[i].z);

		//glUniform1i(texMode_uniformId, 0); 	

		processObject(meshes[candle_id]);
	}
}

void renderScene(void) {

	GLint loc;

	FrameCount++;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	// set the camera using a function similar to gluLookAt
	lookAt(camX, camY, camZ, 0,0,0, 0,1,0);
	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates
	//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 

	float res[4];
	multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(lPos_uniformId, 1, res);

	//add main direct light
	addDirectionalLight(0, 1.0f, 12.0f, 1.0f);


	//build table
	renderTable();

	renderCar();

	renderCandles();


	//Render text (bitmap fonts) in screen coordinates. So use ortoghonal projection with viewport coordinates.
	glDisable(GL_DEPTH_TEST);
	//the glyph contains background colors and non-transparent for the actual character pixels. So we use the blending
	glEnable(GL_BLEND);  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	//viewer at origin looking down at  negative z direction
	pushMatrix(MODEL);
	loadIdentity(MODEL);
	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	pushMatrix(VIEW);
	loadIdentity(VIEW);
	ortho(m_viewport[0], m_viewport[0] + m_viewport[2] - 1, m_viewport[1], m_viewport[1] + m_viewport[3] - 1, -1, 1);
	RenderText(shaderText, "This is a sample text", 25.0f, 25.0f, 1.0f, 0.5f, 0.8f, 0.2f);
	RenderText(shaderText, "CGJ Light and Text Rendering Demo", 440.0f, 570.0f, 0.5f, 0.3, 0.7f, 0.9f);
	popMatrix(PROJECTION);
	popMatrix(VIEW);
	popMatrix(MODEL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glutSwapBuffers();
}

// ------------------------------------------------------------
//
// Events from the Keyboard
//

void processKeys(unsigned char key, int xx, int yy)
{
	switch(key) {

		case 27:
			glutLeaveMainLoop();
			break;
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': if (globalLight == 1)globalLight = 0; else globalLight = 1; break;
		case 'c': if (pointLights == 1)pointLights = 0; else pointLights = 1; break;
		case 'h': if (carLights == 1)carLights = 0; else carLights = 1; break;
	}
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux = 0;
	float betaAux = 0;
	float rAux = 0;

	deltaX =  - xx + startX;
	deltaY =    yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

//  uncomment this if not using an idle or refresh func
//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

//  uncomment this if not using an idle or refresh func
//	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/phong-blinn.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/phong-blinn.frag");


	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	//glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");

	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");

	
	printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	// Shader for bitmap Text
	shaderText.init();
	shaderText.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/text.vert");
	shaderText.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/text.frag");

	glLinkProgram(shaderText.getProgramIndex());
	printf("InfoLog for Text Rendering Shader\n%s\n\n", shaderText.getAllInfoLogs().c_str());
	
	return(shader.isProgramLinked() && shaderText.isProgramLinked());
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//


void initTable() {
	MyMesh amesh;

	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.8f, 0.6f, 0.4f, 1.0f };
	float spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;

	//create legs + cover
	amesh = createCube();
		
	memcpy(amesh.mat.ambient, amb, 4 * sizeof(float));
	memcpy(amesh.mat.diffuse, diff, 4 * sizeof(float));
	memcpy(amesh.mat.specular, spec, 4 * sizeof(float));
	memcpy(amesh.mat.emissive, emissive, 4 * sizeof(float));
	amesh.mat.shininess = shininess;
	amesh.mat.texCount = texcount;
	
	meshes.push_back(amesh);

	table_id = meshes.size() - 1;
}

void initWheel() {
	MyMesh amesh;

	float amb[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diff[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float spec[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 20.0f;
	int texcount = 0;


	amesh = createTorus(1.0f - radius, 1.0f, 20, 20);
	memcpy(amesh.mat.ambient, amb, 4 * sizeof(float));
	memcpy(amesh.mat.diffuse, diff, 4 * sizeof(float));
	memcpy(amesh.mat.specular, spec, 4 * sizeof(float));
	memcpy(amesh.mat.emissive, emissive, 4 * sizeof(float));
	amesh.mat.shininess = shininess;
	amesh.mat.texCount = texcount;
	
	meshes.push_back(amesh);

	wheel_id = meshes.size() - 1;
}

void initCar() {
	MyMesh amesh;

	float amb[] = { 1.0f, 110.0f/255.0f, 25.0f / 255.0f, 1.0f };
	float diff[] = { 1.0f, 110.0f / 255.0f, 25.0f / 255.0f, 1.0f };
	float spec[] = { 1.0f, 110.0f / 255.0f, 25.0f / 255.0f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;


	amesh = createTorus(1.0f - radius, 1.0f, 20, 20);
	memcpy(amesh.mat.ambient, amb, 4 * sizeof(float));
	memcpy(amesh.mat.diffuse, diff, 4 * sizeof(float));
	memcpy(amesh.mat.specular, spec, 4 * sizeof(float));
	memcpy(amesh.mat.emissive, emissive, 4 * sizeof(float));
	amesh.mat.shininess = shininess;
	amesh.mat.texCount = texcount;

	meshes.push_back(amesh);

	car_id = meshes.size() - 1;
}

void initCandle(int i, int id, float x, float z) {
	Candles[i].x = x;
	Candles[i].z = z;
	Candles[i].id = id;
}

void initCandles() {
	MyMesh amesh;
	float amb[] = { 1.0f, 233.0f / 255.0f, 164.0f / 255.0f, 1.0f };
	float diff[] = { 1.0f, 233.0f / 255.0f, 164.0f / 255.0, 1.0f };
	float spec[] = { 1.0f, 233.0f / 255.0f, 164.0f / 255.0, 1.0f };
	float emissive[] = { 1.0f, 233.0f / 255.0f, 164.0f / 255.0, 1.0f };
	float shininess = 40.0f;
	int texcount = 0;


	amesh = createCylinder(2, 0.5, 20);
	memcpy(amesh.mat.ambient, amb, 4 * sizeof(float));
	memcpy(amesh.mat.diffuse, diff, 4 * sizeof(float));
	memcpy(amesh.mat.specular, spec, 4 * sizeof(float));
	memcpy(amesh.mat.emissive, emissive, 4 * sizeof(float));
	amesh.mat.shininess = shininess;
	amesh.mat.texCount = texcount;
	meshes.push_back(amesh);

	candle_id = meshes.size() - 1;

	// Add hand-picked positions to the Candles[] array
	initCandle(0, 1, -1, 6);
	initCandle(1, 2, 10, -10);
	initCandle(2, 3, 9, 7);
	initCandle(3, 4, -10, 10);
	initCandle(4, 5, -9, -7);
	initCandle(5, 6, -1, -6);
}

void init()
{
	MyMesh amesh;

	/* Initialization of DevIL */
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		printf("wrong DevIL version \n");
		exit(0);
	}
	ilInit();

	/// Initialization of freetype library with font_name file
	freeType_init(font_name);

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);


	// Store scene objects' properties
	initTable();
	initWheel();
	initCar();
	initCandles();
	
	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char** argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);


	//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutTimerFunc(0, timer, 0);
	//glutIdleFunc(renderScene);	// Use it for maximum performance
	glutTimerFunc(0, refresh, 0);   //use it to to get 60 FPS whatever

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);


	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);
}