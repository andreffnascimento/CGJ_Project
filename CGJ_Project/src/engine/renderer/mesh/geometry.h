#ifndef __engine_renderer_mesh_geometry__
#define __engine_renderer_mesh_geometry__


#include "engine/renderer/vsShaderLib.h"
#include <GL/glew.h>



enum texType {DIFFUSE, SPECULAR, NORMALS, BUMP};

struct Material{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shininess;
};

// A model can be made of many meshes. Each is stored  in the following structure
struct MyMesh {
		GLuint vao;
		GLuint texUnits[VSShaderLib::MAX_TEXTURES];
		texType texTypes[4];
		float transform[16];
		GLuint numIndexes;
		unsigned int type;
		struct Material mat;
};

MyMesh createCube();
MyMesh createCubeWithTexCoords(float u, float v);
MyMesh createQuad(float size_x, float size_y);
MyMesh createSphere(float radius, int divisions);
MyMesh createTorus(float innerRadius, float outerRadius, int rings, int sides);
MyMesh createCylinder(float height, float radius, int sides);
MyMesh createCone(float height, float baseRadius, int sides);
MyMesh createPawn();
MyMesh computeVAO(int numP, float *p, float *pfloatoints, int sides, float smoothCos);
void multiplyCubeTexCoordsByAspectRatio(float u, float v, float result[]);
int revSmoothNormal2(float *p, float *nx, float *ny, float smoothCos, int beginEnd);
float *circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX= 0.0f, float transY = 0.0f);
void ComputeTangentArray(int vertexCount, float *vertex, float *normal, float *texcoord, GLuint indexesCount, GLuint *faceIndex, float *tangent);


#endif // !__engine_renderer_mesh_geometry__