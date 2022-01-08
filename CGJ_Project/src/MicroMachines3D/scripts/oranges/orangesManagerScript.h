#ifndef __mm3d_scripts_oranges_orangesManagerScript__
#define __mm3d_scripts_oranges_orangesManagerScript__

#include <random>

#include "MicroMachines3D/common/include.h"


#include "MicroMachines3D/entities/world/orange.h"
//#include "MicroMachines3D/scripts/orange/orangeMovementScript.h"



class OrangesManagerScript : public Script
{


private:
	//std::mt19937 _gen(std::random_device);

	//std::default_random_engine _gen;
	//std::uniform_int_distribution<> _spawnRangeX;
	//std::uniform_int_distribution<> _spawnRangeZ;
	//std::uniform_int_distribution<> _speedRange;
	//std::uniform_int_distribution<> _directionRange;

	std::list<Orange> _orangeObjects = std::list<Orange>();
	std::list<OrangeMovementScript> _orangeScriptComponents = std::list<OrangeMovementScript>();

public:
	OrangesManagerScript() = delete;
	OrangesManagerScript(const OrangesManagerScript&) = default;
	OrangesManagerScript(Scene* scene): Script(scene) {}
	~OrangesManagerScript() = default;


public:
	void onCreate() override
	{
		Coords3f position, movementDirection;
		float speed;

		//std::random_device rd;
		//_gen.seed(21423545345);

		//_spawnRangeX(ORANGE_SPAWN_AREA.x, ORANGE_SPAWN_AREA.y);
		//_spawnRangeZ(ORANGE_SPAWN_AREA.z, ORANGE_SPAWN_AREA.w);
		//_speedRange(MIN_ORANGE_BASE_SPEED, MAX_ORANGE_BASE_SPEED);
		//_directionRange(ORANGE_DIRECTION_MIN, ORANGE_DIRECTION_MAX);


		for (int i = 0; i < NUM_ORANGES; i++)
		{
			position = getRandomSpawnPosition();
			movementDirection = getRandomMovementDirection();
			speed = getRandomBaseSpeed();
			 
			Orange orange = (createOrange(i, position, movementDirection, speed));

			_orangeObjects.push_back(orange);
			_orangeScriptComponents.push_back(orange.getComponent<OrangeMovementScript>());
		}
	}

	void onUpdate(float ts) override
	{
		for each (OrangeMovementScript orange in _orangeScriptComponents)
		{
			if (orange.isOutsideOfBorders()) 
			{
				respawnOrange(orange);
			}
		}
	}

private:
	Orange createOrange(int index, Coords3f position, Coords3f movementDirection, float speed)
	{
		std::string tag = "orange" + std::to_string(index);

		return Orange(_scene, tag, position, movementDirection, speed);
	}

	void respawnOrange(OrangeMovementScript orange)
	{
		Coords3f position, movementDirection;

		position = getRandomSpawnPosition();
		movementDirection = getRandomMovementDirection();

		orange.setMovementDirection(movementDirection);
		orange.respawnAtPosition(position);
	}

	Coords3f getRandomSpawnPosition() 
	{
		//float coordX = _spawnRangeX(_gen);
		//float coordZ = _spawnRangeZ(_gen);

		float coordX = 0.0f;
		float coordZ = 0.0f;

		Coords3f position = { coordX, ORANGE_RADIUS, coordZ };

		return position;
	}

	float getRandomBaseSpeed()
	{
		//return _speedRange(_gen);
		return 200.0f;
	}

	Coords3f getRandomMovementDirection()
	{
		//float dirX = _directionRange(_gen);
		float dirX = 1.0f;
		float dirY = 0.0f;
		float dirZ = 1.0f;
		//float dirZ = _directionRange(_gen);

		Coords3f direction = { dirX, dirY, dirZ };

		return direction.normalize();
	}
};

#endif // !__mm3d_scripts_oranges_orangesManagerScript__