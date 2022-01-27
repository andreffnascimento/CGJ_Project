#ifndef __engine_utils_importer__
#define __engine_utils_importer__


#include "engine/scene/components.h"




class Importer
{



public:
	static void importModel(ModelComponent&, const char* modelPath);

};


#endif // !__engine_utils_importer__