#ifndef __mm3d_scripts_oranges_orangesManagerScript__
#define __mm3d_scripts_oranges_orangesManagerScript__


#include <list>
#include <random>

#include "MicroMachines3D/common/include.h"




class OrangesManagerScript : public Script
{

private:
	enum class OrangeState
	{
		SPAWNING	= 0,	// oranges are increasing in size
		MOVING		= 1,	// oranges are moving
		DESPAWNING	= 2,	// oranges are reducing in size
		N_STATES    = 3
	};


private:
	struct OrangeData
	{
		Entity orange = Entity();
		RigidbodyComponent* orangeRigidbody = nullptr;
		OrangeState orangeState = OrangesManagerScript::OrangeState::SPAWNING;
		int spawnTableSide = -1;
	};


private:
	typedef void (OrangesManagerScript::*process_oranges_state_func_t)(OrangeData&, float);


	

private:
	constexpr static float MIN_VELOCITY = 1.0f;
	constexpr static float MAX_VELOCITY = 10.0f;
	constexpr static float MAX_ANGLE_VARIATION = 75.0f;

	constexpr static float SIZE_TO_MOVE = 4.0f;
	constexpr static float SIZE_TO_RESPAWN = 0.2f;
	constexpr static float INCREASE_SIZE_FACTOR = 500.0f;
	constexpr static float DECREASE_SIZE_FACTOR = 0.002f;

	constexpr static float INCREASE_TIME_FACTOR = 0.1f;

	constexpr static float ORANGE_ROTATION_SPEED_FACTOR = 20.0f;




private:
	std::mt19937 _randomGenerator;
	std::uniform_int_distribution<int> _spawnTableSide;
	std::uniform_real_distribution<float> _spawnPositionX;
	std::uniform_real_distribution<float> _spawnPositionZ;
	std::uniform_real_distribution<float> _spawnVelocity;
	std::uniform_real_distribution<float> _spawnAngle;

	std::list<OrangeData> _orangesData = std::list<OrangeData>();

	process_oranges_state_func_t _processOrangesStateFuncs[(unsigned int)OrangeState::N_STATES] = {};

	float _timeModifier = 1.0f;		// used to increase the speed of the oranges as the game progresses




public:
	OrangesManagerScript() = delete;
	OrangesManagerScript(const OrangesManagerScript&) = default;
	OrangesManagerScript(Scene* scene): Script(scene, "OrangesManagerScript") {}
	~OrangesManagerScript() = default;


public:
	void onCreate() override
	{
		// initializes the random generator
		std::random_device randomDevice;
		_randomGenerator = std::mt19937(randomDevice());
		_spawnTableSide = std::uniform_int_distribution<int>(0, 3); // (negativeZ ; negativeX ; positiveZ ; positiveX)
		_spawnPositionX = std::uniform_real_distribution<float>(-TABLE_SIZE.x / 2.0f, TABLE_SIZE.x / 2.0f);
		_spawnPositionZ = std::uniform_real_distribution<float>(-TABLE_SIZE.z / 2.0f, TABLE_SIZE.z / 2.0f);
		_spawnVelocity = std::uniform_real_distribution<float>(OrangesManagerScript::MIN_VELOCITY, OrangesManagerScript::MAX_VELOCITY);
		_spawnAngle = std::uniform_real_distribution<float>(-OrangesManagerScript::MAX_ANGLE_VARIATION, OrangesManagerScript::MAX_ANGLE_VARIATION);
		
		// initializes the oranges processing function array
		_processOrangesStateFuncs[(unsigned int)OrangeState::SPAWNING]	 = &OrangesManagerScript::_spawnOrange;
		_processOrangesStateFuncs[(unsigned int)OrangeState::MOVING]	 = &OrangesManagerScript::_moveOrange;
		_processOrangesStateFuncs[(unsigned int)OrangeState::DESPAWNING] = &OrangesManagerScript::_despawnOrange;

		// finds all the oranges in the game
		std::unordered_set<Entity> orangeEntities = _scene->getEntitiesByTag(std::regex("^Oranges:orange_[0-9]+$"));
		for (auto& orangeEntity : orangeEntities)
			_orangesData.push_back({ orangeEntity, &orangeEntity.getComponent<RigidbodyComponent>() });

		// initializes all the oranges
		for (auto& orangeData : _orangesData)
			_generateOrangeInfo(orangeData);
	}


	void onUpdate(float ts) override
	{
		_timeModifier += ts * OrangesManagerScript::INCREASE_TIME_FACTOR;

		for (auto& orangeData : _orangesData)
			(this->*_processOrangesStateFuncs[(unsigned int)orangeData.orangeState])(orangeData, ts);
	}




public:
	void resetOrangesSpeed()
	{
		_timeModifier = 1.0f;
	}




private:
	void _generateOrangeInfo(OrangeData& orangeData)
	{
		orangeData.spawnTableSide = _spawnTableSide(_randomGenerator);
		orangeData.orangeState = OrangesManagerScript::OrangeState::SPAWNING;
		Transform::translateTo(orangeData.orange, _calculateOrangeSpawnCoords(orangeData.spawnTableSide));
		Transform::scaleTo(orangeData.orange, Coords3f({ SIZE_TO_RESPAWN, SIZE_TO_RESPAWN, SIZE_TO_RESPAWN }));
		Transform::rotateTo(orangeData.orange, Coords3f({ 0.0f, 0.0f, 0.0f }));
	}


	void _spawnOrange(OrangeData& orangeData, float ts)
	{
		float increaseFactor = std::pow(OrangesManagerScript::INCREASE_SIZE_FACTOR, ts);
		Transform::scale(orangeData.orange, Coords3f({ increaseFactor, increaseFactor, increaseFactor}));
		if (orangeData.orange.transform().scale().x > SIZE_TO_MOVE)
		{
			float directionAngle = orangeData.spawnTableSide * 90.0f + _spawnAngle(_randomGenerator);
			Coords3f orangeVelocity = _calculateOrangeSpawnVelocity(directionAngle);
			Coords3f orangeAngularVelocity = Coords3f({ orangeVelocity.length() / (ORANGE_SIZE.x * 0.5f), 0.0f, 0.0f });
			orangeData.orangeState = OrangesManagerScript::OrangeState::MOVING;
			orangeData.orangeRigidbody->setVelocity(orangeVelocity);
			orangeData.orangeRigidbody->setAngularVelocity(orangeAngularVelocity * ORANGE_ROTATION_SPEED_FACTOR);
			Transform::scaleTo(orangeData.orange, ORANGE_SIZE);
			Transform::rotateTo(orangeData.orange, Coords3f({ 0.0f, directionAngle, 0.0f }));
		}
	}


	void _moveOrange(OrangeData& orangeData, float ts)
	{
		const Coords3f& orangePosition = orangeData.orangeRigidbody->position();
		if (std::abs(orangePosition.x) > (TABLE_SIZE.x) / 2.0f || std::abs(orangePosition.z) > (TABLE_SIZE.z) / 2.0f)
		{
			orangeData.orangeState = OrangesManagerScript::OrangeState::DESPAWNING;
			orangeData.orangeRigidbody->setVelocity(Coords3f({ 0.0f, 0.0f, 0.0f }));
		}
	}


	void _despawnOrange(OrangeData& orangeData, float ts)
	{
		float decreaseFactor = std::pow(OrangesManagerScript::DECREASE_SIZE_FACTOR, ts);
		Transform::scale(orangeData.orange, Coords3f({ decreaseFactor, decreaseFactor, decreaseFactor }));
		if (orangeData.orange.transform().scale().x < OrangesManagerScript::SIZE_TO_RESPAWN)
			_generateOrangeInfo(orangeData);
	}




private:

	Coords3f _calculateOrangeSpawnCoords(int spawnTableSide)
	{
		Coords3f spawnCoords = Coords3f({ 0.0f, ORANGE_SIZE.y / 2.0f, 0.0f });
		float spawnTableSideSign = spawnTableSide < 2 ? -1.0f : 1.0f;
		if (spawnTableSide & 1)
		{
			spawnCoords.x = spawnTableSideSign * TABLE_SIZE.x / 2.0f;
			spawnCoords.z = _spawnPositionZ(_randomGenerator);
		}
		else
		{
			spawnCoords.x = _spawnPositionX(_randomGenerator);
			spawnCoords.z = spawnTableSideSign * TABLE_SIZE.z / 2.0f;
		}
		return spawnCoords;
	}


	Coords3f _calculateOrangeSpawnVelocity(float directionAngle)
	{
		Coords3f spawnDirection = Coords3f({ 0.0f, 0.0f, 1.0f });
		Quaternion velocityRotation(Coords3f({ 0.0f, 1.0f, 0.0f }), directionAngle);
		velocityRotation.rotatePoint(spawnDirection);
		spawnDirection *= _timeModifier * _spawnVelocity(_randomGenerator);
		return spawnDirection;
	}

};

#endif // !__mm3d_scripts_oranges_orangesManagerScript__