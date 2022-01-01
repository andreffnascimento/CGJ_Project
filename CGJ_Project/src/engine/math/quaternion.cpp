#include "quaternion.h"

#include "engine/utils/mathUtils.h"




Quaternion::Quaternion(float x, float y, float z, float w)
	: _x(x), _y(y), _z(z), _w(w)
{
	normalize();
}

Quaternion::Quaternion(const Coords4f& coords)
	: _x(coords.x), _y(coords.y), _z(coords.z), _w(coords.w)
{
	normalize();
}

Quaternion::Quaternion(const Coords3f& axis, float angle)
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm

	float sinHalfAngle = std::sin(toRadians(angle) * 0.5f);
	float cosHalfAngle = std::cos(toRadians(angle) * 0.5f);

	_x = axis.x * sinHalfAngle;
	_y = axis.y * sinHalfAngle;
	_z = axis.z * sinHalfAngle;
	_w = cosHalfAngle;
}

Quaternion::Quaternion(const Coords3f& rotation)
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm

	float halfCosHeading = std::cos(toRadians(rotation.y) / 2.0f);
	float halfSinHeading = std::sin(toRadians(rotation.y) / 2.0f);
	float halfCosAttitude = std::cos(toRadians(rotation.z) / 2.0f);
	float halfSinAttitude = std::sin(toRadians(rotation.z) / 2.0f);
	float halfCosBank = std::cos(toRadians(rotation.x) / 2.0f);
	float halfSinBank = std::sin(toRadians(rotation.x) / 2.0f);

	float halfCosHeading_halfCosAttitude = halfCosHeading * halfCosAttitude;
	float halfSinHeading_halfSinAttitude = halfSinHeading * halfSinAttitude;

	_w = halfCosHeading_halfCosAttitude   * halfCosBank - halfSinHeading_halfSinAttitude   * halfSinBank;
	_x = halfCosHeading_halfCosAttitude   * halfSinBank + halfSinHeading_halfSinAttitude   * halfCosBank;
	_y = halfSinHeading * halfCosAttitude * halfCosBank + halfCosHeading * halfSinAttitude * halfSinBank;
	_z = halfCosHeading * halfSinAttitude * halfCosBank - halfSinHeading * halfCosAttitude * halfSinBank;
}




Coords3f Quaternion::forward() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	return { 2.0f * (_x * _z - _w * _y), 2.0f * (_y * _z + _w * _x), 1.0f - 2.0f * (_x * _x + _y * _y) };
}


Coords3f Quaternion::right() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	return { 1.0f - 2.0f * (_y * _y + _z * _z), 2.0f * (_x * _y - _w * _z), 2.0f * (_x * _z + _w * _y) };
}


Coords3f Quaternion::up() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	return { 2.0f * (_x * _y + _w * _z), 1.0f - 2.0f * (_x * _x + _z * _z), 2.0f * (_y * _z - _w * _x) };
}


Coords3f Quaternion::toEulerAngles() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm

	float test = _x * _y + _z * _w;

	if (test > 0.499f)	// singularity at north pole
		return Coords3f({ toDegrees(2.0f * std::atan2(_x, _w)),		toDegrees((float)HALF_PI),		0.0f });

	if (test < -0.499f) // singularity at south pole
		return Coords3f({ toDegrees(-2.0f * std::atan2(_x, _w)),	toDegrees(-(float)HALF_PI),		0.0f });
	
	Coords3f eulerAngles = Coords3f();
	float sqx = _x * _x;
	float sqy = _y * _y;
	float sqz = _z * _z;

	eulerAngles.y = toDegrees(std::atan2(2.0f * (_y * _w - _x * _z), 1.0f - 2.0f * (sqy - sqz)));
	eulerAngles.z = toDegrees(std::asin(2.0f * test));
	eulerAngles.x = toDegrees(std::atan2(2.0f * (_x * _w - _y * _z), 1.0f - 2.0f * (sqx * sqz)));

	return eulerAngles;
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
	return Quaternion(_x * invLength, _y * invLength, _z * invLength, _w * invLength);
}




Quaternion& Quaternion::operator=(const Quaternion& other)
{
	_x = other._x;
	_y = other._y;
	_z = other._z;
	_w = other._w;

	return *this;
}

Quaternion& Quaternion::operator=(const Coords3f& rotation)
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm

	float halfCosHeading = std::cos(toRadians(rotation.y) / 2.0f);
	float halfSinHeading = std::sin(toRadians(rotation.y) / 2.0f);
	float halfCosAttitude = std::cos(toRadians(rotation.z) / 2.0f);
	float halfSinAttitude = std::sin(toRadians(rotation.z) / 2.0f);
	float halfCosBank = std::cos(toRadians(rotation.x) / 2.0f);
	float halfSinBank = std::sin(toRadians(rotation.x) / 2.0f);

	float halfCosHeading_halfCosAttitude = halfCosHeading * halfCosAttitude;
	float halfSinHeading_halfSinAttitude = halfSinHeading * halfSinAttitude;

	_w = halfCosHeading_halfCosAttitude * halfCosBank - halfSinHeading_halfSinAttitude * halfSinBank;
	_x = halfCosHeading_halfCosAttitude * halfSinBank + halfSinHeading_halfSinAttitude * halfCosBank;
	_y = halfSinHeading * halfCosAttitude * halfCosBank + halfCosHeading * halfSinAttitude * halfSinBank;
	_z = halfCosHeading * halfSinAttitude * halfCosBank - halfSinHeading * halfCosAttitude * halfSinBank;

	return *this;
}



void Quaternion::operator+=(const Quaternion& other)
{
	_x += other._x;
	_y += other._y;
	_z += other._z;
	_w += other._w;
	normalize();
}

void Quaternion::operator+=(const Coords3f& rotation)
{
	Coords3f eulerAngles = toEulerAngles();
	eulerAngles += rotation;
	*this = eulerAngles;
}



void Quaternion::operator-=(const Quaternion& other)
{
	_x -= other._x;
	_y -= other._y;
	_z -= other._z;
	_w -= other._w;
	normalize();
}

void Quaternion::operator-=(const Coords3f& rotation)
{
	Coords3f eulerAngles = toEulerAngles();
	eulerAngles -= rotation;
	*this = eulerAngles;
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

void Quaternion::operator*=(const Coords3f& rotation)
{
	Coords3f eulerAngles = toEulerAngles();
	eulerAngles *= rotation;
	*this = eulerAngles;
}



Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1._x + q2._x, q1._y + q2._y, q1._z + q2._z, q1._w + q2._w);
}

Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1._x - q2._x, q1._y - q2._y, q1._z - q2._z, q1._w - q2._w);
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
