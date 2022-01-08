#ifndef __mm3d_scripts_oranges_orangesManagerScript__
#define __mm3d_scripts_oranges_orangesManagerScript__

#include <random>

#include "MicroMachines3D/common/include.h"


#include "MicroMachines3D/entities/world/orange.h"
//#include "MicroMachines3D/scripts/orange/orangeMovementScript.h"



class OrangesManagerScript : public Script
{
private:
	std::list<Orange> _orangeObjects = std::list<Orange>();
	std::list<OrangeMovementScript> _orangeScriptComponents = std::list<OrangeMovementScript>();
	std::mt19937 _gen;


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

		std::random_device rd;
		_gen.seed(rd);

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
		std::uniform_int_distribution<> rangeX(ORANGE_SPAWN_AREA.x, ORANGE_SPAWN_AREA.y);
		std::uniform_int_distribution<> rangeZ(ORANGE_SPAWN_AREA.z, ORANGE_SPAWN_AREA.w);

		float coordX = rangeX(_gen);
		float coordZ = rangeZ(_gen);

		Coords3f position = { coordX, ORANGE_RADIUS, coordZ };

		return position;
	}

	float getRandomBaseSpeed()
	{
		std::uniform_int_distribution<> speedRange(MIN_ORANGE_BASE_SPEED, MAX_ORANGE_BASE_SPEED);

		return speedRange(_gen);
	}

	Coords3f getRandomMovementDirection()
	{
		std::uniform_int_distribution<> range(-5, 5);

		float dirX = range(_gen);
		float dirY = 0;
		float dirZ = range(_gen);

		Coords3f direction = { dirX, dirY, dirZ };

		return direction.normalize();
	}
};

#endif // !__mm3d_scripts_oranges_orangesManagerScript__