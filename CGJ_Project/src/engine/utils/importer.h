#ifndef __engine_utils_importer__
#define __engine_utils_importer__


#include "engine/scene/components.h"




class Importer
{

public:
	static void importModel(ModelComponent& model, const char* modelPath);
	static void importLensFlare(LensFlareComponent& lensFlare, const char* lensFlarePath);




private:
	static int _importLensFlareColorMap(const std::string& lensFlareDir, const std::string& textureName, int flareTextureIds[]);

};


#endif // !__engine_utils_importer__