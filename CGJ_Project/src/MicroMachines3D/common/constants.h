#ifndef __mm3d_common_constants__
#define __mm3d_common_constants__


#include "engine/utils/coords.h"




constexpr bool GRAVITY_ENABLED = true;




constexpr Coords3f TABLE_SIZE = { 150.0f, 5.0f, 75.0f };
constexpr Coords3f LEG_SIZE = { 5.0f, 50.0f, 5.0f };




constexpr float CAR_FLOOR_DISTANCE = 0.35f;
constexpr float CAR_SPAWN_HEIGHT = GRAVITY_ENABLED ? 10.0f : 0.0f;

constexpr Coords3f CAR_BOTTOM_SIZE = { 1.5f, 1.0f, 5.0f };
constexpr Coords3f CAR_TOP_SIZE = { CAR_BOTTOM_SIZE.x, CAR_BOTTOM_SIZE.y / 2.0f, CAR_BOTTOM_SIZE.z / 2.5f };
constexpr Coords3f CAR_SPOILER_SUPPORT_SIZE = { 0.2f, 0.5f, 0.2f };
constexpr Coords3f CAR_SPOILER_SIZE = { 2.0f, 0.2f, 0.5f };
constexpr Coords3f CAR_BUMPER_SIZE = { 1.5f, 0.4f, 0.2f };
constexpr Coords3f CAR_HEADLIGHT_SIZE = { 0.3f, 0.15f, 0.1f };
constexpr Coords3f WHEEL_SIZE = { 0.5f, 1.0f, 0.5f };

constexpr float WHEEL_INNER_RADIUS = 0.55f;
constexpr float WHEEL_OUTER_RADIUS = 1.0f;
constexpr int   WHEEL_RINGS = 20;
constexpr int   WHEEL_SIDES = 20;




constexpr float CHEERIO_INNER_RADIUS = 0.15f;
constexpr float CHEERIO_OUTER_RADIUS = 0.375f;
constexpr int   CHEERIO_RINGS = 20;
constexpr int   CHEERIO_SIDES = 20;

constexpr Coords3f CHEERIO_SIZE = { 3.45f, 6.0f, 3.45f };

constexpr float CHEERIO_SPACING = 2.0f * CHEERIO_OUTER_RADIUS * CHEERIO_SIZE.x;
constexpr float TRACK_WIDTH = 6.0f * CHEERIO_SPACING;




constexpr float CANDEL_HEIGHT = 1.0f;
constexpr float CANDEL_RADIUS = 0.5f;
constexpr int   CANDEL_SIDES = 20;

constexpr Coords3f CANDEL_SIZE = { 3.0f, 5.0f, 3.0f };
constexpr Coords3f CANDEL_TOP_SIZE = { 0.3f, 1.0f, 0.3f };




constexpr float ORANGE_RADIUS = 0.5f;
constexpr int ORANGE_DIVISIONS = 20;

constexpr Coords3f ORANGE_SIZE = { 6.0f, 6.0f, 6.0f };

constexpr int N_ORANGES = 7;




constexpr float BUTTER_HEIGHT = 0.25f;



constexpr Coords3f HEART_SIZE = { 60.0f, 60.0f, 1.0f };
constexpr Coords3f PAUSE_SIZE = { 140.0f, 100.0f, 1.0f };
constexpr Coords3f GASPEDAL_SIZE = { 30.0f, 80.0f, 1.0f };




constexpr unsigned int N_PARTICLES = 2000;
constexpr float PARTICLE_SIZE = 2.0f;
constexpr float PARTICLE_INITIAL_HEIGHT = 10.0f;


#endif // !__mm3d_common_constants__