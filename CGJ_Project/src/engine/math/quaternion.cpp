#include "quaternion.h"

#include "engine/utils/mathUtils.h"




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

Quaternion::Quaternion(const Coords3f& rotation)
{
	// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	float halfCosRoll = std::cos(toRadians(rotation.x * 0.5f));
	float halfSinRoll = std::sin(toRadians(rotation.x * 0.5f));
	float halfCosPitch = std::cos(toRadians(rotation.y * 0.5f));
	float halfSinPitch = std::sin(toRadians(rotation.y * 0.5f));
	float halfCosYaw = std::cos(toRadians(rotation.z * 0.5f));
	float halfSinYaw = std::sin(toRadians(rotation.z * 0.5f));

	_w = halfCosRoll * halfCosPitch * halfCosYaw + halfSinRoll * halfSinPitch * halfSinYaw;
	_x = halfSinRoll * halfCosPitch * halfCosYaw - halfCosRoll * halfSinPitch * halfSinYaw;
	_y = halfCosRoll * halfSinPitch * halfCosYaw + halfSinRoll * halfCosPitch * halfCosYaw;
	_z = halfCosRoll * halfCosPitch * halfSinYaw - halfSinRoll * halfSinPitch * halfCosYaw;
}




Quaternion& Quaternion::normalize()
{
	float invLength = invSqrt(length2());
	_x = _x * invLength;
	_y = _y * invLength;
	_z = _z * invLength;
	_w = _w * invLength;
	return *this;
}


Quaternion Quaternion::normalized() const
{
	float invLength = invSqrt(length2());
	return Quaternion(Coords4f({ _x * invLength, _y * invLength, _z * invLength, _w * invLength }));
}




Coords3f Quaternion::toEulerAngles() const
{
	// Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	Coords3f eulerAngles = Coords3f();

	// roll (x-axis rotation)
	float sinRoll = 2.0f * (_w * _x + _y * _z);
	float cosRoll = 1.0f - 2.0f * (_x * _x + _y * _y);
	eulerAngles.x = std::atan2(sinRoll, cosRoll);

	// pitch (y-axis rotation)
	float sinPitch = 2.0f * (_w * _y - _z * _x);
	eulerAngles.y = std::abs(sinPitch) >= 1 ? std::copysign(PI / 2.0, sinPitch) : std::asin(sinPitch);

	// yaw (z-axis rotation)
	float sinYaw = 2.0f * (_w * _z + _x * _y);
	float cosYaw = 1.0f - 2.0f * (_y * _y + _z * _z);
	eulerAngles.z = std::atan2(sinYaw, cosYaw);

	return eulerAngles;
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
