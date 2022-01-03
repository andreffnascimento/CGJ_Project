#ifndef __engine_math_quaternion__
#define __engine_math_quaternion__


#include <cmath>

#include "engine/utils/coords.h"


class TransformMatrix;




class Quaternion
{

private:
	double _x = 0.0;
	double _y = 0.0;
	double _z = 0.0;
	double _w = 1.0;


public:
	Quaternion() = default;
	Quaternion(const Quaternion&) = default;
	Quaternion(double x, double y, double z, double w);
	Quaternion(const Coords4f& coords);
	Quaternion(const Coords3f& axis, float angle);
	Quaternion(const Coords3f& rotation);
	Quaternion(const TransformMatrix& rotationMatrix);
	~Quaternion() = default;


public:
	inline float length() const							{ return std::sqrt(length2()); }
	inline float length2() const						{ return (float)(_x * _x + _y * _y + _z * _z + _w * _w); }
	inline float dot(const Quaternion& other) const		{ return (float)(_x * other._x + _y * other._y + _z * other._z + _w * other._w); }
	

public:
	Coords3f forward() const;
	Coords3f right() const;
	Coords3f up() const;

	Coords3f toEulerAngles() const;


public:
	Quaternion& rotate(const Coords3f& eulerAngles);
	Quaternion& rotate(const Quaternion& quaternion);
	Quaternion& normalize();

	Quaternion normalized() const;
	Quaternion conjugated() const;

	Coords3f& rotatePoint(Coords3f& point) const;
	Coords3f calculatePointRotation(const Coords3f& point) const;


public:
	Quaternion& operator=(const Quaternion& other);
	Quaternion& operator=(const Coords3f& rotation);

	void operator+=(const Quaternion& other);
	void operator-=(const Quaternion& other);
	void operator*=(const Quaternion& other);

	friend Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
	friend Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
	friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

};


#endif // !__engine_math_quaternion__