#ifndef __mm3d_common_constants__
#define __mm3d_common_constants__

#include "MicroMachines3D/common/include.h"


constexpr Coords3f TABLE_SIZE = { 180.0f, 5.0f, 100.0f };
constexpr Coords3f LEG_SIZE = { 5.0f, 50.0f, 5.0f };


constexpr float CAR_FLOOR_DISTANCE = 0.35f;

constexpr Coords3f CAR_BOTTOM_SIZE = { 1.5f, 1.0f, 5.0f };
constexpr Coords3f CAR_TOP_SIZE = { CAR_BOTTOM_SIZE.x, CAR_BOTTOM_SIZE.y / 2.0f, CAR_BOTTOM_SIZE.z / 2.5f };
constexpr Coords3f CAR_SPOILER_SUPPORT_SIZE = { 0.2f, 0.5f, 0.2f };
constexpr Coords3f CAR_SPOILER_SIZE = { 2.0f, 0.2f, 0.5f };
constexpr Coords3f CAR_BUMPER_SIZE = { 1.5f, 0.4f, 0.2f };
constexpr Coords3f WHEEL_SIZE = { 0.5f, 1.0f, 0.5f };

constexpr float WHEEL_INNER_RADIUS = 0.55f;
constexpr float WHEEL_OUTER_RADIUS = 1.0f;
constexpr int   WHEEL_RINGS = 20;
constexpr int   WHEEL_SIDES = 20;


#endif // !__mm3d_common_constants__