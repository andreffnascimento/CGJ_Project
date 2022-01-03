#include "transformMatrix.h"

#include <algorithm>




TransformMatrix::TransformMatrix()
{
	setIdentityMatrix();
}

TransformMatrix::TransformMatrix(const TransformMatrix& matrix)
{
	memcpy(_matrix, matrix._matrix, 4 * 4 * sizeof(float));
}




float TransformMatrix::determinant() const
{
	float det00 = _matrix[0][0] * (_matrix[1][1] * _matrix[2][2] - _matrix[1][2] * _matrix[2][1]);
	float det01 = _matrix[0][1] * (_matrix[1][0] * _matrix[2][2] - _matrix[1][2] * _matrix[2][0]);
	float det02 = _matrix[0][2] * (_matrix[1][0] * _matrix[2][1] - _matrix[1][1] * _matrix[2][0]);
	return det00 - det01 + det02;
}




const TransformMatrix& TransformMatrix::calculateTransformMatrix(const Coords3f& translation, const Quaternion& rotation, const Coords3f& scale)
{
	setIdentityMatrix();

	TransformMatrix translationMatrix = TransformMatrix();
	TransformMatrix rotationMatrix = TransformMatrix();
	TransformMatrix scaleMatrix = TransformMatrix();

	translationMatrix.setTranslationMatrix(translation);
	rotationMatrix.setRotationMatrix(rotation).transpose();
	scaleMatrix.setScaleMatrix(scale);
	
	*this = translationMatrix * rotationMatrix * scaleMatrix;
	return *this;
}




TransformMatrix& TransformMatrix::setIdentityMatrix()
{
	_matrix[0][0] = 1.0f;	_matrix[0][1] = 0.0f;	_matrix[0][2] = 0.0f;	_matrix[0][3] = 0.0f;
	_matrix[1][0] = 0.0f;	_matrix[1][1] = 1.0f;	_matrix[1][2] = 0.0f;	_matrix[1][3] = 0.0f;
	_matrix[2][0] = 0.0f;	_matrix[2][1] = 0.0f;	_matrix[2][2] = 1.0f;	_matrix[2][3] = 0.0f;
	_matrix[3][0] = 0.0f;	_matrix[3][1] = 0.0f;	_matrix[3][2] = 0.0f;	_matrix[3][3] = 1.0f;

	return *this;
}


TransformMatrix& TransformMatrix::setTranslationMatrix(const Coords3f& translation)
{
	_matrix[3][0] = translation.x;
	_matrix[3][1] = translation.y;
	_matrix[3][2] = translation.z;

	return *this;
}


TransformMatrix& TransformMatrix::setRotationMatrix(const Quaternion& rotation)
{
	// Source: https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm

	Coords3f f = rotation.forward();
	Coords3f r = rotation.right();
	Coords3f u = rotation.up();

	_matrix[0][0] = r.x;	_matrix[0][1] = r.y;	_matrix[0][2] = r.z;
	_matrix[1][0] = u.x;	_matrix[1][1] = u.y;	_matrix[1][2] = u.z;
	_matrix[2][0] = f.x;	_matrix[2][1] = f.y;	_matrix[2][2] = f.z;

	return *this;
}


TransformMatrix& TransformMatrix::setScaleMatrix(const Coords3f& scale)
{
	_matrix[0][0] = scale.x;
	_matrix[1][1] = scale.y;
	_matrix[2][2] = scale.z;

	return *this;
}




TransformMatrix& TransformMatrix::transpose()
{
	for (int i = 0; i < 4; i++)
		for (int j = i + 1; j < 4; j++)
			std::swap(_matrix[i][j], _matrix[j][i]);

	return *this;
}




TransformMatrix operator*(const TransformMatrix& m1, const TransformMatrix& m2)
{
	TransformMatrix res = TransformMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[j][i] =  m1[0][i] * m2[j][0];
			res[j][i] += m1[1][i] * m2[j][1];
			res[j][i] += m1[2][i] * m2[j][2];
			res[j][i] += m1[3][i] * m2[j][3];
		}
	}

	return res;
}

Coords4f operator*(const TransformMatrix& m, const Coords4f& p)
{
	Coords4f res = Coords4f();

	res.x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z;
	res.y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z;
	res.z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z;

	return res;
}