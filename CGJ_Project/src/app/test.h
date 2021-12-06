#pragma once

#include "inputHandler.h"
#include "game/game.h"
#include "math/AVTmathLib.h"
#include "renderer/shaderHelper.h"
#include "text/avtFreeType.h"
#include "renderer/geometry.h"


float table_length = 100.0f;
float table_width = 40.0f;
float table_thickness = 2.5f;

float leg_length = 25.0f;
float leg_thickness = 3.0f;


// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// DevIL library
#include <IL/il.h>



extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

MyMesh initTable() {
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

	return amesh;
}


void processObject(MyMesh obj) {
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(getUniformLocation(UniformType::VM), 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(getUniformLocation(UniformType::PVM), 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(getUniformLocation(UniformType::NORMAL), 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(obj.vao);

	if (!getShader().isProgramValid()) {
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

	MyMesh obj = initTable();

	loc = glGetUniformLocation(getShader().getProgramIndex(), "ambient");
	glUniform4fv(loc, 1, obj.mat.ambient);
	loc = glGetUniformLocation(getShader().getProgramIndex(), "diffuse");
	glUniform4fv(loc, 1, obj.mat.diffuse);
	loc = glGetUniformLocation(getShader().getProgramIndex(), "specular");
	glUniform4fv(loc, 1, obj.mat.specular);
	loc = glGetUniformLocation(getShader().getProgramIndex(), "shininess");
	glUniform1f(loc, obj.mat.shininess);

	//Cover
	pushMatrix(MODEL);
	scale(MODEL, table_width, table_thickness, table_length);

	processObject(obj);

	//Legs
	renderLegs(0, -leg_length, 0);
	processObject(obj);

	renderLegs(0, -leg_length, table_length - leg_thickness);
	processObject(obj);

	renderLegs(table_width - leg_thickness, -leg_length, table_length - leg_thickness);
	processObject(obj);

	renderLegs(table_width - leg_thickness, -leg_length, 0);
	processObject(obj);
}