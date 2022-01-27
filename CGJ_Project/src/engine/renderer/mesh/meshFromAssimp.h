#ifndef __engine_renderer_meshFromAssimp__
#define __engine_renderer_meshFromAssimp__


#include "assimp/Importer.hpp"	
#include "assimp/postprocess.h"
#include "assimp/scene.h"




const aiScene* Import3DFromFile(const std::string& pFile);
std::vector<struct MyMesh> createMeshFromAssimp(const aiScene* sc);


#endif // !__engine_renderer_meshFromAssimp__