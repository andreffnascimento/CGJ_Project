#ifndef __MM3D_COMMON_CONSTANTS_H__
#define __MM3D_COMMON_CONSTANTS_H__


constexpr float TABLE_LENGTH = 180.0f;
constexpr float TABLE_WIDTH = 100.0f;
constexpr float TABLE_THICKNESS = 5.0f;

constexpr float LEG_LENGTH = 50.0f;
constexpr float LEG_THICKNESS = 5.0f;



constexpr float CAR_BOTTOM_WIDTH = 1.5f;
constexpr float CAR_BOTTOM_HEIGHT = 1.0f;
constexpr float CAR_BOTTOM_LENGTH = 5.0f;

constexpr float CAR_TOP_WIDTH  = CAR_BOTTOM_WIDTH;
constexpr float CAR_TOP_HEIGHT = CAR_BOTTOM_HEIGHT / 2.0f;
constexpr float CAR_TOP_LENGTH = CAR_BOTTOM_LENGTH / 2.5f;

constexpr float WHEEL_INNER_RADIUS = 0.55f;
constexpr float WHEEL_OUTER_RADIUS = 1.0f;
constexpr int   WHEEL_RINGS = 20;
constexpr int   WHEEL_SIDES = 20;

constexpr float WHEEL_WIDTH = 0.5f;
constexpr float WHEEL_HEIGHT = 1.0f;
constexpr float WHEEL_LENGTH = 0.5f;


#endif // !__MM3D_COMMON_CONSTANTS_H__