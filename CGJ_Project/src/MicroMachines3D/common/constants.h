#ifndef __mm3d_common_constants__
#define __mm3d_common_constants__


#include "engine/utils/coords.h"




constexpr bool GRAVITY_ENABLED = true;




constexpr Coords3f TABLE_SIZE = { 150.0f, 5.0f, 75.0f };
constexpr Coords3f LEG_SIZE = { 5.0f, 50.0f, 5.0f };




constexpr float CAR_FLOOR_DISTANCE = 0.35f;
constexpr float CAR_SPAWN_HEIGHT = GRAVITY_ENABLED ? 10.0f : 0.0f;
constexpr float CAR_SCALE = 0.08f;

constexpr Coords3f CAR_SIZE = { 3.0f, 2.8f, 7.7f };



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


constexpr Coords3f MIRROR_SIZE = { 400.0f, 150.0f, 1.0f };



constexpr float BUTTER_HEIGHT = 0.25f;



constexpr Coords3f HEART_SIZE = { 60.0f, 60.0f, 1.0f };
constexpr Coords3f PAUSE_SIZE = { 140.0f, 100.0f, 1.0f };
constexpr Coords3f GASPEDAL_SIZE = { 30.0f, 80.0f, 1.0f };




constexpr unsigned int N_FIREWORKS = 2000;
constexpr float FIREWORK_SIZE = 2.0f;
constexpr float FIREWORK_INITIAL_HEIGHT = 12.0f;
constexpr float FIREWORK_SPEED = 5.0f;

constexpr Coords3f FIREWORK_PARTICLE_SIZE = { 0.2f, 0.2f, 0.2f };



constexpr Coords3f SKYBOX_SIZE = { 300.0f, 300.0f, 300.0f };



constexpr float PLANET_SUN_SIZE = 25.0f;
constexpr float PLANET_EARTH_SIZE = 3.0f;
constexpr float PLANET_MARS_SIZE = 2.5f;
constexpr float PLANET_JUPITER_SIZE = 9.0f;
constexpr float PLANET_SATURN_SIZE = 8.0f;
constexpr float PLANET_NEPTUNE_SIZE = 6.5f;
constexpr float PLANET_PLUTO_SIZE = 1.5f;

constexpr Coords3f PLANET_SUN_POSITION = { 0.0f, 300.0f, -500.0f };
constexpr Coords3f PLANET_EARTH_POSITION = { -175.0f, 15.0f, 15.0f };
constexpr Coords3f PLANET_MARS_POSITION = { -300.0f, 30.0f, -65.0f };
constexpr Coords3f PLANET_JUPITER_POSITION = { 350.0f, 10.0f, -80.0f };
constexpr Coords3f PLANET_SATURN_POSITION = { 450.0f, 30.0f, 80.0f };
constexpr Coords3f PLANET_NEPTUNE_POSITION = { 100.0f, 40.0f, 300.0f };
constexpr Coords3f PLANET_PLUTO_POSITION = { -50.0f, 20.0f, 100.0f };


#endif // !__mm3d_common_constants__