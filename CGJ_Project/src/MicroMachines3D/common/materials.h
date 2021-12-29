#ifndef __mm3d_common_materials__
#define __mm3d_common_materials__


#include "engine/renderer/geometry.h"


constexpr Material TABLE_MATERIAL = {
	{ 0.2f, 0.1f, 0.0f, 1.0f },
	{ 0.3f, 0.2f, 0.0f, 1.0f },
	{ 0.8f, 0.8f, 0.8f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
	0
};




constexpr Material CAR_BODY_MATERIAL = {
	{ 0.2f, 0.2f, 0.2f, 1.0f },
	{ 0.4f, 0.4f, 0.4f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	100.0f,
	0
};


constexpr Material CAR_WHEEL_MATERIAL = {
	{ 0.1f, 0.1f, 0.1f, 1.0f },
	{ 0.1f, 0.1f, 0.1f, 1.0f },
	{ 0.4f, 0.4f, 0.4f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	20.0f,
	0
};


constexpr Material CAR_HEADLIGHT_FRONT_OFF_MATERIAL = {
	{ 0.2f, 0.2f, 0.0f, 1.0f },
	{ 0.3f, 0.3f, 0.1f, 1.0f },
	{ 0.8f, 0.8f, 0.3f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	80.0f,
	0
};

constexpr Material CAR_HEADLIGHT_BACK_OFF_MATERIAL = {
	{ 0.2f, 0.0f, 0.0f, 1.0f },
	{ 0.3f, 0.0f, 0.0f, 1.0f },
	{ 0.8f, 0.3f, 0.3f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	80.0f,
	0
};

constexpr Material CAR_HEADLIGHT_FRONT_ON_MATERIAL = {
	{ 0.8f, 0.8f, 0.0f, 1.0f },
	{ 0.8f, 0.8f, 0.0f, 1.0f },
	{ 0.8f, 0.8f, 0.3f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	80.0f,
	0
};

constexpr Material CAR_HEADLIGHT_BACK_ON_MATERIAL = {
	{ 0.9f, 0.0f, 0.1f, 1.0f },
	{ 0.9f, 0.0f, 0.3f, 1.0f },
	{ 0.9f, 0.3f, 0.3f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 0.0f },
	80.0f,
	0
};




constexpr Material CHEERIO_MATERIAL = {
	{ 0.34f, 0.29f, 0.11f, 1.0f },
	{ 0.80f, 0.56f, 0.00f, 1.0f },
	{ 0.95f, 0.95f, 0.72f, 1.0f },
	{ 0.00f, 0.00f, 0.00f, 0.0f },
	10.0f,
	0
};


#endif // !__mm3d_common_materials__
