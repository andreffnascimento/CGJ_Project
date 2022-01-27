#include "importer.h"

#include <iostream>
#include <fstream>

#include "engine/utils/mathUtils.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/mesh/meshFromAssimp.h"


extern const aiScene* scene;




void Importer::importModel(ModelComponent& modelComponent, const char* modelPath)
{
	std::ifstream modelFile(modelPath);
	if (!modelFile.fail())
		modelFile.close();
	else
		throw std::string("Unable to open the model file with the path: \"" + std::string(modelPath) + "\"\n" + std::string(strerror(errno)) + "\n");

	if ((modelComponent._scene = Import3DFromFile(modelPath)) == nullptr)
		throw std::string("Unable to import the model with the path: \"" + std::string(modelPath) + "\"");

	modelComponent._meshes = createMeshFromAssimp(modelComponent._scene);
	std::cout << "Model file: \"" << std::string(modelPath) << "\" loaded.";
}


void Importer::importLensFlare(LensFlareComponent& lensFlare, const char* lensFlarePath)
{
	FILE* lensFlareFile = fopen(lensFlarePath, "r");
	if (!lensFlareFile)
		throw std::string("Unable to open the lens flare file with the path: \"" + std::string(lensFlarePath) + "\"\n" + std::string(strerror(errno)) + "\n");

	int flareTextureIds[RendererSettings::MAX_FLARE_TEXTURES] = { -1, -1, -1, -1 };
	constexpr size_t READ_BUFFER_SIZE = 1024;
	char readBuffer[1024];

	LensFlareComponent::FlareData& lensFlareData = lensFlare._flareData;
	fgets(readBuffer, READ_BUFFER_SIZE, lensFlareFile);
	if (sscanf(readBuffer, "%f %f", &lensFlareData.scale, &lensFlareData.maxElementSize) != 2)
		throw std::string("Invalid format in the lens flare file with the path: \"" + std::string(lensFlarePath) + "\"");

	while (!feof(lensFlareFile))
	{
		char name[8] = { '\0', };
		double distance = 0.0;
		double size = 0.0;
		float color[4];

		fgets(readBuffer, READ_BUFFER_SIZE, lensFlareFile);
		if (sscanf(readBuffer, "%4s %lf %lf ( %f %f %f %f )", name, &distance, &size, &color[3], &color[0], &color[1], &color[2]) != 7)
			throw std::string("Invalid format in the lens flare file with the path: \"" + std::string(lensFlarePath) + "\"");

		std::string lensFlareDir = std::string(lensFlarePath);
		lensFlareDir.erase(lensFlareDir.rfind('/') + 1);

		LensFlareComponent::FlareElement flareElement = lensFlareData.elements[lensFlareData.nElements++];
		flareElement.color = { clamp(color[0] / 255.0f, 0.0f, 1.0f), clamp(color[1] / 255.0f, 0.0f, 1.0f), clamp(color[2] / 255.0f, 0.0f, 1.0f), clamp(color[3] / 255.0f, 0.0f, 1.0f) };
		flareElement.distance = (float)distance;
		flareElement.size = (float)size;
		flareElement.textureId = Importer::_importLensFlareTexture(lensFlareDir.c_str(), name, flareTextureIds);
		if (flareElement.textureId == -1)
			throw std::string("Invalid texture name \"" + std::string(name) + "\" in the lens flare file with the path: \"" + std::string(lensFlarePath) + "\"");
		
	}

	std::cout << "Flare file: \"" << std::string(lensFlarePath) <<  "\" loaded.";
	fclose(lensFlareFile);
}




int Importer::_importLensFlareTexture(const std::string& lensFlareDir, const std::string& textureName, int flareTextureIds[])
{
	constexpr char* flareTextureNames[RendererSettings::MAX_FLARE_TEXTURES] = { "crcl", "hxgn", "ring", "sun", "flar" };
	for (int i = 0; i < RendererSettings::MAX_FLARE_TEXTURES; i++)
	{
		if (textureName.compare(flareTextureNames[i]) == 0)
		{
			if (flareTextureIds[i] == -1)
				flareTextureIds[i] = Renderer::create2dTexture(std::string(lensFlareDir + textureName + std::string(".tga")).c_str());

			return (unsigned int)flareTextureIds[i];
		}
	}

	return -1;
}