#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H


#include "game/gameObject.h"


class Camera : public GameObject
{

public:
	struct Coords
	{
		float x;
		float y;
		float z;
	};



private:
	// camera position
	Coords _coords;

	// camera spherical coordinates
	float _alpha;
	float _beta;
	float _r;



public:
	Camera();


public:
	inline const Coords& getCoords() const { return _coords; }


public:
	friend class WorldCameraScript;
};


#endif // !GAME_CAMERA_H
