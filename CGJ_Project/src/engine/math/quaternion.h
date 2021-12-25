#ifndef __engine_math_quaternion__
#define __engine_math_quaternion__


#include "engine/utils/coords.h"




class Quaternion
{

private:
	float _x = 0.0f;
	float _y = 0.0f;
	float _z = 0.0f;
	float _w = 1.0f;


public:
	Quaternion() = default;
	Quaternion(const Quaternion&) = default;
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Coords4f& coords);
	Quaternion(const Coords3f& axis, float angle);
	~Quaternion() = default;


public:
	float lenght() const;
	Quaternion& normalize();


public:
	Coords3f forward() const;
	Coords3f right() const;
	Coords3f up() const;


public:
	void operator*=(const Quaternion& other);
	friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

};


#endif // !__engine_math_quaternion__
