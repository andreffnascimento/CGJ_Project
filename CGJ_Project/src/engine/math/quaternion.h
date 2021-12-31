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
	Quaternion(const Coords4f& coords);
	Quaternion(const Coords3f& axis, float angle);
	Quaternion(const Coords3f& rotation);
	~Quaternion() = default;


public:
	inline float length() const		{ return std::sqrt(length2()); }
	inline float length2() const	{ return _x * _x + _y * _y + _z * _z + _w * _w; }

	Quaternion& normalize();
	Quaternion normalized() const;

	Coords3f toEulerAngles() const;


public:
	Coords3f forward() const;
	Coords3f right() const;
	Coords3f up() const;


public:
	void operator*=(const Quaternion& other);
	friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

};


#endif // !__engine_math_quaternion__