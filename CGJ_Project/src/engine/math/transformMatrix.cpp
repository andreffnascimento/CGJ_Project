#include "transformMatrix.h"

#include <cmath>
#include <cstring>
#include <algorithm>

#include "engine/math/angle.h"
#include "engine/math/quaternion.h"




TransformMatrix::TransformMatrix()
{
	setIdentityMatrix();
}

TransformMatrix::TransformMatrix(const TransformMatrix& matrix)
{
	memcpy(_matrix, matrix._matrix, 4 * 4 * sizeof(float));
}




const TransformMatrix& TransformMatrix::calculateTransformMatrix(const Coords3f& translation, const Coords3f& rotation, const Coords3f& scale)
{
	setIdentityMatrix();

	TransformMatrix translationMatrix = TransformMatrix();
	TransformMatrix rotationMatrix = TransformMatrix();
	TransformMatrix scaleMatrix = TransformMatrix();

	translationMatrix.setTranslationMatrix(translation);
	rotationMatrix.setRotationMatrix(rotation);
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


TransformMatrix& TransformMatrix::setRotationMatrix(const Coords3f& rotation)
{
	Quaternion rotationQuaternion = Quaternion();

	if (rotation.x != 0.0f)
	{
		Quaternion xQuaternion = Quaternion({ 1.0f, 0.0f, 0.0f }, rotation.x);
		rotationQuaternion *= xQuaternion;
	}

	if (rotation.y != 0.0f)
	{
		Quaternion yQuaternion = Quaternion({ 0.0f, 1.0f, 0.0f }, rotation.y);
		rotationQuaternion *= yQuaternion;
	}

	if (rotation.z != 0.0f)
	{
		Quaternion zQuaternion = Quaternion({ 0.0f, 0.0f, 1.0f }, rotation.z);
		rotationQuaternion *= zQuaternion;
	}

	Coords3f f = rotationQuaternion.forward();
	Coords3f r = rotationQuaternion.right();
	Coords3f u = rotationQuaternion.up();

	_matrix[0][0] = r.x;	_matrix[0][1] = r.y;	_matrix[0][2] = r.z;
	_matrix[1][0] = u.x;	_matrix[1][1] = u.y;	_matrix[1][2] = u.z;
	_matrix[2][0] = f.x;	_matrix[2][1] = f.y;	_matrix[2][2] = f.z;
	
	return this->transpose();
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