#ifndef __engine_math_quaternion__
#define __engine_math_quaternion__


#include "engine/utils/coords.h"




class Quaternion
{

public:
	float _x = 0.0;
	float _y = 0.0;
	float _z = 0.0;
	float _w = 1.0;


public:
	Quaternion() = default;
	Quaternion(const Quaternion&) = default;
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Coords4f& coords);
	Quaternion(const Coords3f& axis, float angle);
	Quaternion(const Coords3f& rotation);
	~Quaternion() = default;


public:
	inline float length() const							{ return std::sqrt(length2()); }
	inline float length2() const						{ return _x * _x + _y * _y + _z * _z + _w * _w; }
	inline float dot(const Quaternion& other) const		{ return _x * other._x + _y * other._y + _z * other._z + _w * other._w; }
	

public:
	Coords3f forward() const;
	Coords3f right() const;
	Coords3f up() const;

	Coords3f toEulerAngles() const;


public:
	Quaternion& normalize();
	Quaternion normalized() const;


public:
	Quaternion& operator=(const Quaternion& other);
	Quaternion& operator=(const Coords3f& rotation);

	void operator+=(const Quaternion& other);
	void operator+=(const Coords3f& rotation);

	void operator-=(const Quaternion& other);
	void operator-=(const Coords3f& rotation);

	void operator*=(const Quaternion& other);
	void operator*=(const Coords3f& rotation);

	friend Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
	friend Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
	friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

};


#endif // !__engine_math_quaternion__