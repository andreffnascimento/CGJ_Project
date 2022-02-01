
#ifndef __mm3d_common_materials__
#define __mm3d_common_materials__


#include "engine/renderer/mesh/geometry.h"




constexpr Material TABLE_MATERIAL = {
	{ 0.2f, 0.1f, 0.0f, 1.0f },
	{ 0.3f, 0.2f, 0.0f, 0.7f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
};


constexpr Material FLOATING_CUBE_MATERIAL = {
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	100.0f,
};


constexpr Material CHEERIO_MATERIAL = {
	{ 0.1f, 0.1f, 0.1f, 1.0f },
	{ 0.2f, 0.2f, 0.2f, 1.0f },
	{ 0.2f, 0.2f, 0.2f, 1.0f },
	{ 0.00f, 0.00f, 0.00f, 0.0f },
	10.0f,
};




constexpr Material CANDEL_OFF_MATERIAL = {
	{ 0.4f, 0.1f, 0.0f, 1.0f },
	{ 0.8f, 0.2f, 0.1f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
};


constexpr Material CANDEL_ON_MATERIAL = {
	{ 1.0f, 0.4f, 0.4f, 1.0f },
	{ 1.0f, 0.4f, 0.4f, 1.0f },
	{ 1.0f, 0.4f, 0.4f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
};




constexpr Material ORANGE_MATERIAL = {
	{ 0.5f, 0.25f, 0.0f, 1.0f },
	{ 1.0f, 0.5f,  0.0f, 1.0f },
	{ 1.0f, 0.5f,  0.0f, 1.0f },
	{ 0.0f, 0.0f,  0.0f, 0.0f },
	10.0f,
};




constexpr Material BUTTER_MATERIAL = {
	{ 0.30f, 0.30f, 0.10f, 1.0f },
	{ 0.80f, 0.80f, 0.10f, 0.4f },
	{ 0.90f, 0.90f, 0.20f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
};


constexpr Material BOOSTER_MATERIAL = {
	{ 0.09f, 0.25f, 0.0f, 1.0f },
	{ 0.03f, 0.80f, 0.0f, 0.4f },
	{ 0.90f, 0.90f, 0.20f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
};




constexpr Material FLAG_MATERIAL = {
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.90f, 0.90f, 0.20f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
};


constexpr Material MIRROR_MATERIAL = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	100.0f,
};

#endif // !__mm3d_common_materials__