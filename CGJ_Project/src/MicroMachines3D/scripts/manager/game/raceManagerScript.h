#ifndef __mm3d_scripts_manager_game_raceManagerScript__
#define __mm3d_scripts_manager_game_raceManagerScript__


#include "MicroMachines3D/common/include.h"

#include <ctime>

#include "MicroMachines3D/scripts/car/carMovementScript.h"
#include "MicroMachines3D/scripts/oranges/orangesManagerScript.h"




class RaceManagerScript : public Script
{

private:
	enum class GameState
	{
		PLAYING = 0,
		PAUSED = 1,
		GAMEOVER = 2,
		RESPAWN = 3
	};




private:
	static constexpr unsigned int MAX_LIVES = 5;
	static constexpr float RESET_GAME_HEIGHT = -10.0f;
	static constexpr float RESET_QUERY_TIMER = 2.0f;
	static constexpr float RESET_FIREWORKS_TIMER = 10.0f;


	

private:
	const EventHandler* _eventHandler = nullptr;

	Entity _car = Entity();
	RigidbodyComponent* _carRigidbody = nullptr;

	CanvasComponent* _playingScreenCanvas = nullptr;
	CanvasComponent* _pauseScreenCanvas = nullptr;
	CanvasComponent* _gameoverScreenCanvas = nullptr;

	ImageComponent* _hearts[RaceManagerScript::MAX_LIVES] = {};
	TextComponent* _resetQuery = nullptr;
	float _resetQueryTimer = 0.0f;

	ParticleGeneratorComponent* _fireworksGenerator = nullptr;
	bool _collidedWithFlag = false;
	float _resetFireworksTimer = 0.0f;

	CarMovementScript* _carMovementScript = nullptr;
	OrangesManagerScript* _orangesManagerScript = nullptr;

	bool _fogToggle = true;
	bool _bumpToggle = true;

	RaceManagerScript::GameState _gameState = RaceManagerScript::GameState::PLAYING;
	unsigned int _lives = RaceManagerScript::MAX_LIVES;
	bool _collidedWithOrangeLastFrame = false;
	bool _collidedWithOrange = false;


public:
	RaceManagerScript() = delete;
	RaceManagerScript(const RaceManagerScript&) = default;
	RaceManagerScript(Scene* scene) : Script(scene, "RaceManagerScript") {}
	~RaceManagerScript() = default;


public:
	void onCreate() override
	{
		std::srand((unsigned int)std::time(nullptr));

		_eventHandler = &Application::getEventHandler();
		_car = _scene->getEntityByTag("Car");
		_carRigidbody = &_car.getComponent<RigidbodyComponent>();
		_playingScreenCanvas = &_scene->getEntityByTag("PlayingScreen").getComponent<CanvasComponent>();
		_pauseScreenCanvas = &_scene->getEntityByTag("PauseScreen").getComponent<CanvasComponent>();
		_gameoverScreenCanvas = &_scene->getEntityByTag("GameoverScreen").getComponent<CanvasComponent>();
		_carMovementScript = dynamic_cast<CarMovementScript*>(_car.getComponent<ScriptComponent>().getScriptByTag("CarMovementScript"));
		_orangesManagerScript = dynamic_cast<OrangesManagerScript*>(_scene->getEntityByTag("Oranges").getComponent<ScriptComponent>().getScriptByTag("OrangesManagerScript"));

		_resetQuery = &_scene->getEntityByTag("PlayingScreen:ResetQuery").getComponent<TextComponent>();
		for (int i = 0; i < RaceManagerScript::MAX_LIVES; i++)
			_hearts[i] = &_scene->getEntityByTag("PlayingScreen:Heart_" + std::to_string(i)).getComponent<ImageComponent>();

		_fireworksGenerator = &_scene->getEntityByTag("Fireworks").getComponent<ParticleGeneratorComponent>();

		_respawn();
	}


	void onUpdate(float ts) override
	{
		switch (_gameState)
		{

		case RaceManagerScript::GameState::PLAYING:
			_checkPlayingActions(ts);
			break;

		case RaceManagerScript::GameState::PAUSED:
			_checkPauseActions(ts);
			break;

		case RaceManagerScript::GameState::GAMEOVER:
			_checkGameoverActions(ts);
			break;

		case RaceManagerScript::GameState::RESPAWN:
			_respawn();
			break;

		}
	}




public:
	inline bool playing() const		{ return _gameState == RaceManagerScript::GameState::PLAYING; }
	inline bool paused() const		{ return _gameState == RaceManagerScript::GameState::PAUSED; }
	inline bool gameover() const	{ return _gameState == RaceManagerScript::GameState::GAMEOVER; }


public:
	void setCollidedWithOranges(bool colliderWithOranges) { _collidedWithOrange = colliderWithOranges; }
	void setCollidedWithFlag(bool colliderWithFlag) { _collidedWithFlag = colliderWithFlag; }




private:
	inline float _frand() { return (float)std::rand() / RAND_MAX; }




private:
	void _checkPlayingActions(float ts)
	{
		if (_eventHandler->keyState('S').pressed() || _eventHandler->keyState('s').pressed())
		{
			_gameState = RaceManagerScript::GameState::PAUSED;
			Application::getInstance().setTimeScale(0.0f);
			_pauseScreenCanvas->setEnabled(true);
		}

		_updateResetQuery(ts);
		if (_eventHandler->keyState('R').pressed() || _eventHandler->keyState('r').pressed())
			_processResetQueryClick(ts);

		if (_eventHandler->keyState('F').pressed() || _eventHandler->keyState('f').pressed())
			Renderer::setFogActive(_fogToggle = !_fogToggle);

		if (_eventHandler->keyState('B').pressed() || _eventHandler->keyState('b').pressed())
			Renderer::setBumpActive(_bumpToggle = !_bumpToggle);

		if (_carRigidbody->position().y < RESET_GAME_HEIGHT)
			_gameover();

		if (!_collidedWithOrange)
			_collidedWithOrangeLastFrame = false;

		if (_collidedWithFlag && _resetFireworksTimer == 0.0f)
			_startFireworks();

		if (_collidedWithOrange && !_collidedWithOrangeLastFrame)
			_decreaseLives();

		_resetFireworksTimer = std::max(_resetFireworksTimer - ts, 0.0f);
		_collidedWithOrange = false;
		_collidedWithFlag = false;
	}


	void _checkPauseActions(float ts)
	{
		if (_eventHandler->keyState('S').pressed() || _eventHandler->keyState('s').pressed())
		{
			_gameState = RaceManagerScript::GameState::PLAYING;
			Application::getInstance().setTimeScale(1.0f);
			_pauseScreenCanvas->setEnabled(false);
		}
	}

	void _checkGameoverActions(float ts)
	{
		if (_eventHandler->keyState('R').pressed() || _eventHandler->keyState('r').pressed())
			_prepareRespawn();
	}




private:
	void _updateResetQuery(float ts)
	{
		if (_resetQueryTimer == 0.0f)
			return;

		_resetQueryTimer += ts;
		if (_resetQueryTimer > RaceManagerScript::RESET_QUERY_TIMER)
		{
			_resetQueryTimer = 0.0f;
			_resetQuery->setEnabled(false);
		}
	}


	void _processResetQueryClick(float ts)
	{
		_resetQuery->setEnabled(true);
		if (_resetQueryTimer > 0.0f)
			_prepareRespawn();
		_resetQueryTimer += ts;
	}


	void _decreaseLives()
	{
		_collidedWithOrangeLastFrame = true;
		_hearts[--_lives]->setEnabled(false);
		if (_lives == 0)
			_gameover();
	}

	void _gameover()
	{
		_gameState = RaceManagerScript::GameState::GAMEOVER;
		Application::getInstance().setTimeScale(0.0f);
		_playingScreenCanvas->setEnabled(false);
		_pauseScreenCanvas->setEnabled(false);
		_gameoverScreenCanvas->setEnabled(true);
	}


	void _prepareRespawn()
	{
		Application::getInstance().setTimeScale(1.0f);
		_gameState = RaceManagerScript::GameState::RESPAWN;
	}

	void _respawn()
	{
		Transform::translateTo(_car, Coords3f({ 0.0f, CAR_BOTTOM_SIZE.y / 2.0f + CAR_FLOOR_DISTANCE + CAR_SPAWN_HEIGHT, TABLE_SIZE.z / 3.0f }));
		Transform::rotateTo(_car, Coords3f({ 0.0f, -90.0f, 0.0f }));
		_carRigidbody->setVelocity(Coords3f({ 0.0f, 0.0f, 0.0f }));
		_carRigidbody->setAngularVelocity(Coords3f({ 0.0f, 0.0f, 0.0f }));
		_carMovementScript->reset();
		_orangesManagerScript->resetOranges();

		_lives = RaceManagerScript::MAX_LIVES;
		_collidedWithOrange = false;
		_gameState = RaceManagerScript::GameState::PLAYING;
		_playingScreenCanvas->setEnabled(true);
		_pauseScreenCanvas->setEnabled(false);
		_gameoverScreenCanvas->setEnabled(false);

		_resetQuery->setEnabled(false);
		_resetQueryTimer = 0.0f;
		_resetFireworksTimer = RaceManagerScript::RESET_FIREWORKS_TIMER;
		for (int i = 0; i < RaceManagerScript::MAX_LIVES; i++)
			_hearts[i]->setEnabled(true);
	}




private:
	void _startFireworks()
	{
		_resetFireworksTimer = RaceManagerScript::RESET_FIREWORKS_TIMER;
		_fireworksGenerator->setEnabled(true);
		for (unsigned int i = 0; i < _fireworksGenerator->nParticles(); i++)
		{
			float v = 0.8f * _frand() + 0.2f;
			float phi = _frand() * (float)PI;
			float theta = 2.0f * _frand() * (float)PI;
			
			ParticleGeneratorComponent::ParticleData& particle = _fireworksGenerator->particle(i);
			particle.position = Coords3f({ 0.0f, PARTICLE_INITIAL_HEIGHT, TABLE_SIZE.z / 3.0f });
			particle.velocity = Coords3f({ v * cos(theta) * sin(phi), v * cos(phi), v * sin(theta) * sin(phi) });
			particle.acceleration = Coords3f({ 0.1f, -0.15f, 0.0f });
			particle.color = Coords3f({ _frand(), _frand(), _frand() });
			particle.fadeSpeed = 0.15f +_frand() / 5.0f;
			particle.life = 1.0f;
		}
	}

};


#endif // !__mm3d_scripts_manager_game_raceManagerScript__