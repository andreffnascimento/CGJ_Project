#include "importer.h"

#include <iostream>
#include <fstream>

#include "engine/renderer/mesh/meshFromAssimp.h"


extern const aiScene* scene;




void Importer::importModel(ModelComponent& modelComponent, const char* modelPath)
{
	std::ifstream fin(modelPath);
	if (!fin.fail())
		fin.close();
	else
		throw std::string("Couldn't open file: " + std::string(modelPath) + "\n" + std::string(strerror(errno)) + "\n");

	if ((modelComponent._scene = Import3DFromFile(modelPath)) == nullptr)
		throw std::string("Unable to import the model with the path: \"" + std::string(modelPath) + "\"");

	modelComponent._meshes = createMeshFromAssimp(modelComponent._scene);
}