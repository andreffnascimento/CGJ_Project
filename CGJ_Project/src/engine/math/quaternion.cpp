#include "quaternion.h"

#include <algorithm>

#include "engine/math/transformMatrix.h"
#include "engine/utils/mathUtils.h"




Quaternion::Quaternion(double x, double y, double z, double w)
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

	double sinHalfAngle = std::sin(toRadians(angle) * 0.5);
	double cosHalfAngle = std::cos(toRadians(angle) * 0.5);

	_x = axis.x * sinHalfAngle;
	_y = axis.y * sinHalfAngle;
	_z = axis.z * sinHalfAngle;
	_w = cosHalfAngle;
}

Quaternion::Quaternion(const Coords3f& rotation)
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm

	double halfCosHeading = std::cos(toRadians(rotation.y) / 2.0);
	double halfSinHeading = std::sin(toRadians(rotation.y) / 2.0);
	double halfCosAttitude = std::cos(toRadians(rotation.z) / 2.0);
	double halfSinAttitude = std::sin(toRadians(rotation.z) / 2.0);
	double halfCosBank = std::cos(toRadians(rotation.x) / 2.0);
	double halfSinBank = std::sin(toRadians(rotation.x) / 2.0);

	double halfCosHeading_halfCosAttitude = halfCosHeading * halfCosAttitude;
	double halfSinHeading_halfSinAttitude = halfSinHeading * halfSinAttitude;

	_w = halfCosHeading_halfCosAttitude   * halfCosBank - halfSinHeading_halfSinAttitude   * halfSinBank;
	_x = halfCosHeading_halfCosAttitude   * halfSinBank + halfSinHeading_halfSinAttitude   * halfCosBank;
	_y = halfSinHeading * halfCosAttitude * halfCosBank + halfCosHeading * halfSinAttitude * halfSinBank;
	_z = halfCosHeading * halfSinAttitude * halfCosBank - halfSinHeading * halfCosAttitude * halfSinBank;
}

Quaternion::Quaternion(const TransformMatrix& rotationMatrix)
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm

	_w = std::sqrt(std::max(0.0, 1.0 + rotationMatrix[0][0] + rotationMatrix[1][1] + rotationMatrix[2][2])) / 2.0;
	_x = std::sqrt(std::max(0.0, 1.0 + rotationMatrix[0][0] - rotationMatrix[1][1] - rotationMatrix[2][2])) / 2.0;
	_y = std::sqrt(std::max(0.0, 1.0 - rotationMatrix[0][0] + rotationMatrix[1][1] - rotationMatrix[2][2])) / 2.0;
	_z = std::sqrt(std::max(0.0, 1.0 - rotationMatrix[0][0] - rotationMatrix[1][1] + rotationMatrix[2][2])) / 2.0;

	_x = std::copysign(_x, rotationMatrix[2][1] - rotationMatrix[1][2]);
	_y = std::copysign(_y, rotationMatrix[0][2] - rotationMatrix[2][0]);
	_z = std::copysign(_z, rotationMatrix[1][0] - rotationMatrix[0][1]);
}




Coords3f Quaternion::forward() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	return { (float)(2.0 * (_x * _z - _w * _y)),		(float)(2.0 * (_y * _z + _w * _x)),			(float)(1.0 - 2.0 * (_x * _x + _y * _y)) };
}


Coords3f Quaternion::right() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	return { (float)(1.0 - 2.0 * (_y * _y + _z * _z)),	(float)(2.0 * (_x * _y - _w * _z)),			(float)(2.0 * (_x * _z + _w * _y)) };
}


Coords3f Quaternion::up() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	return { (float)(2.0 * (_x * _y + _w * _z)),		(float)(1.0 - 2.0 * (_x * _x + _z * _z)),	(float)(2.0 * (_y * _z - _w * _x)) };
}



Coords3f Quaternion::toEulerAngles() const
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm

	double sqx = _x * _x;
	double sqy = _y * _y;
	double sqz = _z * _z;
	double sqw = _w * _w;

	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = _x * _y + _z * _w;

	if (test > 0.499 * unit)	// singularity at north pole
		return Coords3f({ 0.0f,		(float)toDegrees(2.0 * std::atan2(_x, _w)),		(float)toDegrees(HALF_PI) });

	if (test < -0.499 * unit)	// singularity at south pole
		return Coords3f({ 0.0f,		(float)toDegrees(-2.0 * std::atan2(_x, _w)),	(float)toDegrees(-HALF_PI) });

	Coords3f eulerAngles = Coords3f();
	eulerAngles.x = (float)toDegrees(atan2(2.0 * _x * _w - 2.0 * _y * _z, -sqx + sqy - sqz + sqw));
	eulerAngles.y = (float)toDegrees(atan2(2.0 * _y * _w - 2.0 * _x * _z,  sqx - sqy - sqz + sqw));
	eulerAngles.z = (float)toDegrees(asin(2.0 * test / unit));
	return eulerAngles;
}




Quaternion& Quaternion::rotate(const Coords3f& eulerAngles)
{
	Coords3f quaternionEulerAngles = toEulerAngles();
	quaternionEulerAngles += eulerAngles;
	return (*this = quaternionEulerAngles);
}

Quaternion& Quaternion::rotate(const Quaternion& quaternion)
{
	*this *= quaternion;
	return *this;
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


Quaternion Quaternion::conjugated() const
{
	return Quaternion(-_x, -_y, -_z, _w);
}



Coords3f& Quaternion::rotatePoint(Coords3f& point) const
{
	Coords4f rotatedPoint = TransformMatrix().setRotationMatrix(*this) * Coords4f({ point.x, point.y, point.z, 0.0f });
	point.x = rotatedPoint.x;
	point.y = rotatedPoint.y;
	point.z = rotatedPoint.z;
	return point;
}


Coords3f Quaternion::calculatePointRotation(const Coords3f& point) const
{
	Coords4f rotatedPoint = TransformMatrix().setRotationMatrix(*this) * Coords4f({ point.x, point.y, point.z, 0.0f });
	return Coords3f({ rotatedPoint.x, rotatedPoint.y, rotatedPoint.z });
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

	double halfCosHeading = std::cos(toRadians(rotation.y) / 2.0f);
	double halfSinHeading = std::sin(toRadians(rotation.y) / 2.0f);
	double halfCosAttitude = std::cos(toRadians(rotation.z) / 2.0f);
	double halfSinAttitude = std::sin(toRadians(rotation.z) / 2.0f);
	double halfCosBank = std::cos(toRadians(rotation.x) / 2.0f);
	double halfSinBank = std::sin(toRadians(rotation.x) / 2.0f);

	double halfCosHeading_halfCosAttitude = halfCosHeading * halfCosAttitude;
	double halfSinHeading_halfSinAttitude = halfSinHeading * halfSinAttitude;

	_w = halfCosHeading_halfCosAttitude   * halfCosBank - halfSinHeading_halfSinAttitude   * halfSinBank;
	_x = halfCosHeading_halfCosAttitude   * halfSinBank + halfSinHeading_halfSinAttitude   * halfCosBank;
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


void Quaternion::operator-=(const Quaternion& other)
{
	_x -= other._x;
	_y -= other._y;
	_z -= other._z;
	_w -= other._w;
	normalize();
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
