#ifndef __mm3d_scripts_manager_game_raceManagerScript__
#define __mm3d_scripts_manager_game_raceManagerScript__


#include "MicroMachines3D/common/include.h"

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


	

private:
	const EventHandler* _eventHandler = nullptr;

	Entity _car = Entity();
	RigidbodyComponent* _carRigidbody = nullptr;

	CanvasComponent* _playingScreenCanvas = nullptr;
	CanvasComponent* _pauseScreenCanvas = nullptr;
	CanvasComponent* _gameoverScreenCanvas = nullptr;
	ImageComponent* _hearts[RaceManagerScript::MAX_LIVES] = {};

	CarMovementScript* _carMovementScript = nullptr;
	OrangesManagerScript* _orangesManagerScript = nullptr;

	bool _fogToggle = true;
	bool _bumpToggle = true;

	RaceManagerScript::GameState _gameState = RaceManagerScript::GameState::PLAYING;
	unsigned int _lives = RaceManagerScript::MAX_LIVES;
	bool _collideLastFrame = false;
	bool _colliderWithOrange = false;


public:
	RaceManagerScript() = delete;
	RaceManagerScript(const RaceManagerScript&) = default;
	RaceManagerScript(Scene* scene) : Script(scene, "RaceManagerScript") {}
	~RaceManagerScript() = default;


public:
	void onCreate() override
	{
		_eventHandler = &Application::getEventHandler();
		_car = _scene->getEntityByTag("Car");
		_carRigidbody = &_car.getComponent<RigidbodyComponent>();
		_playingScreenCanvas = &_scene->getEntityByTag("PlayingScreen").getComponent<CanvasComponent>();
		_pauseScreenCanvas = &_scene->getEntityByTag("PauseScreen").getComponent<CanvasComponent>();
		_carMovementScript = dynamic_cast<CarMovementScript*>(_car.getComponent<ScriptComponent>().getScriptByTag("CarMovementScript"));
		_orangesManagerScript = dynamic_cast<OrangesManagerScript*>(_scene->getEntityByTag("Oranges").getComponent<ScriptComponent>().getScriptByTag("OrangesManagerScript"));

		for (int i = 0; i < RaceManagerScript::MAX_LIVES; i++)
			_hearts[i] = &_scene->getEntityByTag("PlayingScreen:Heart_" + std::to_string(i)).getComponent<ImageComponent>();

		_respawn();
	}


	void onUpdate(float ts) override
	{
		switch (_gameState)
		{

		case RaceManagerScript::GameState::PLAYING:
			_checkPlayingActions();
			break;

		case RaceManagerScript::GameState::PAUSED:
			_checkPauseActions();
			break;

		case RaceManagerScript::GameState::GAMEOVER:
			_checkGameoverActions();
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
	void setColliderWithOranges(bool colliderWithOranges) { _colliderWithOrange = colliderWithOranges; }




private:
	void _checkPlayingActions()
	{
		if (_eventHandler->keyState('S').pressed() || _eventHandler->keyState('s').pressed())
		{
			_gameState = RaceManagerScript::GameState::PAUSED;
			Application::getInstance().setTimeScale(0.0f);
			_pauseScreenCanvas->setEnabled(true);
		}

		if (_eventHandler->keyState('F').pressed() || _eventHandler->keyState('f').pressed())
			Renderer::setFogActive(_fogToggle = !_fogToggle);

		if (_eventHandler->keyState('B').pressed() || _eventHandler->keyState('b').pressed())
			Renderer::setBumpActive(_bumpToggle = !_bumpToggle);

		if (_carRigidbody->position().y < RESET_GAME_HEIGHT)
			_gameover();

		if (!_colliderWithOrange)
			_collideLastFrame = false;

		if (_colliderWithOrange && !_collideLastFrame)
			_decreaseLives();

		_colliderWithOrange = false;
	}


	void _checkPauseActions()
	{
		if (_eventHandler->keyState('S').pressed() || _eventHandler->keyState('s').pressed())
		{
			_gameState = RaceManagerScript::GameState::PLAYING;
			Application::getInstance().setTimeScale(1.0f);
			_pauseScreenCanvas->setEnabled(false);
		}
	}

	void _checkGameoverActions()
	{
		if (_eventHandler->keyState('R').pressed() || _eventHandler->keyState('r').pressed())
			_prepareRespawn();
	}




private:
	void _decreaseLives()
	{
		_collideLastFrame = true;
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
		//_gameoverScreenCanvas->setEnabled(true);
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
		_colliderWithOrange = false;
		_gameState = RaceManagerScript::GameState::PLAYING;
		_playingScreenCanvas->setEnabled(true);
		_pauseScreenCanvas->setEnabled(false);
		//_gameoverScreenCanvas->setEnabled(false);

		for (int i = 0; i < RaceManagerScript::MAX_LIVES; i++)
			_hearts[i]->setEnabled(true);
	}

};


#endif // !__mm3d_scripts_manager_game_raceManagerScript__