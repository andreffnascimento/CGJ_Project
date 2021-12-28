#include "quaternion.h"

#include <cmath>

#include "engine/utils/mathUtils.h"




Quaternion::Quaternion(float x, float y, float z, float w)
	: _x(x), _y(y), _z(z), _w(w)
{
	// empty
}

Quaternion::Quaternion(const Coords4f& coords)
	: _x(coords.x), _y(coords.y), _z(coords.z), _w(coords.w)
{
	// empty
}

Quaternion::Quaternion(const Coords3f& axis, float angle)
{
	float sinHalfAngle = (float)std::sin(toRadians(angle) / 2);
	float cosHalfAngle = (float)std::cos(toRadians(angle) / 2);

	_x = axis.x * sinHalfAngle;
	_y = axis.y * sinHalfAngle;
	_z = axis.z * sinHalfAngle;
	_w = cosHalfAngle;
}




float Quaternion::lenght() const
{
	return (float)(std::sqrt(_x * _x + _y * _y + _z * _z + _w * _w));
}


Quaternion& Quaternion::normalize()
{
	float lenght = this->lenght();
	_x = _x / lenght;
	_y = _y / lenght;
	_z = _z / lenght;
	_w = _w / lenght;
	return *this;
}




Coords3f Quaternion::forward() const
{
	return { 2.0f * (_x * _z - _w * _y), 2.0f * (_y * _z + _w * _x), 1.0f - 2.0f * (_x * _x + _y * _y) };
}


Coords3f Quaternion::right() const
{
	return { 1.0f - 2.0f * (_y * _y + _z * _z), 2.0f * (_x * _y - _w * _z), 2.0f * (_x * _z + _w * _y) };
}


Coords3f Quaternion::up() const
{
	return { 2.0f * (_x * _y + _w * _z), 1.0f - 2.0f * (_x * _x + _z * _z), 2.0f * (_y * _z - _w * _x) };
}




void Quaternion::operator*=(const Quaternion& other)
{
	Quaternion curr = *this;
	_x = curr._x * other._w + curr._w * other._x + curr._y * other._z - curr._z * other._y;
	_y = curr._y * other._w + curr._w * other._y + curr._z * other._x - curr._x * other._z;
	_z = curr._z * other._w + curr._w * other._z + curr._x * other._y - curr._y * other._x;
	_w = curr._w * other._w - curr._x * other._x - curr._y * other._y - curr._z * other._z;
	normalize();
}


Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion quaternion = Quaternion();
	quaternion._x = q1._x * q2._w + q1._w * q2._x + q1._y * q2._z - q1._z * q2._y;
	quaternion._y = q1._y * q2._w + q1._w * q2._y + q1._z * q2._x - q1._x * q2._z;
	quaternion._z = q1._z * q2._w + q1._w * q2._z + q1._x * q2._y - q1._y * q2._x;
	quaternion._w = q1._w * q2._w - q1._x * q2._x - q1._y * q2._y - q1._z * q2._z;
	quaternion.normalize();
	return quaternion;
}
