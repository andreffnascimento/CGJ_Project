#include "quaternion.h"

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

	float trace = rotationMatrix[0][0] + rotationMatrix[1][1] + rotationMatrix[1][1];

	if (trace > 0.0f) 
	{
		double s = std::sqrt(trace + 1.0) * 2;
		_w = 0.25 * s;
		_x = ((double)rotationMatrix[2][1] - (double)rotationMatrix[1][2]) / s;
		_y = ((double)rotationMatrix[0][2] - (double)rotationMatrix[2][0]) / s;
		_z = ((double)rotationMatrix[1][0] - (double)rotationMatrix[0][1]) / s;
	}
	else if ((rotationMatrix[0][0] > rotationMatrix[1][1]) & (rotationMatrix[0][0] > rotationMatrix[2][2])) 
	{
		double s = sqrt(1.0 + rotationMatrix[0][0] - rotationMatrix[1][1] - rotationMatrix[2][2]) * 2;
		_w = ((double)rotationMatrix[2][1] - (double)rotationMatrix[1][2]) / s;
		_x = 0.25 * s;
		_y = ((double)rotationMatrix[0][1] + (double)rotationMatrix[1][0]) / s;
		_z = ((double)rotationMatrix[0][2] + (double)rotationMatrix[2][0]) / s;
	}
	else if (rotationMatrix[1][1] > rotationMatrix[2][2]) 
	{
		double s = sqrt(1.0 + rotationMatrix[1][1] - rotationMatrix[0][0] - rotationMatrix[2][2]) * 2;
		_w = ((double)rotationMatrix[0][2] - (double)rotationMatrix[2][0]) / s;
		_x = ((double)rotationMatrix[0][1] + (double)rotationMatrix[1][0]) / s;
		_y = 0.25 * s;
		_z = ((double)rotationMatrix[1][2] + (double)rotationMatrix[2][1]) / s;
	}
	else 
	{
		double s = sqrt(1.0 + rotationMatrix[2][2] - rotationMatrix[0][0] - rotationMatrix[2][2]) * 2;
		_w = ((double)rotationMatrix[1][0] - (double)rotationMatrix[0][1]) / s;
		_x = ((double)rotationMatrix[0][2] + (double)rotationMatrix[2][0]) / s;
		_y = ((double)rotationMatrix[1][2] + (double)rotationMatrix[2][1]) / s;
		_z = 0.25 * s;
	}
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

	double test = _x * _y + _z * _w;

	if (test > 0.499)	// singularity at north pole
		return Coords3f({ (float)toDegrees(2.0 * std::atan2(_x, _w)),	(float)toDegrees(HALF_PI),	0.0f });

	if (test < -0.499) // singularity at south pole
		return Coords3f({ (float)toDegrees(-2.0 * std::atan2(_x, _w)),	(float)toDegrees(HALF_PI),	0.0f });
	
	Coords3f eulerAngles = Coords3f();
	double sqx = _x * _x;
	double sqy = _y * _y;
	double sqz = _z * _z;

	eulerAngles.y = toDegrees((float)std::atan2(2.0 * (_y * _w - _x * _z), 1.0 - 2.0 * (sqy - sqz)));
	eulerAngles.z = toDegrees((float)std::asin(2.0 * test));
	eulerAngles.x = toDegrees((float)std::atan2(2.0 * (_x * _w - _y * _z), 1.0 - 2.0 * (sqx * sqz)));

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
