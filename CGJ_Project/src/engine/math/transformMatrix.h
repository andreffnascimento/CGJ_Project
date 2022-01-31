#ifndef __engine_math_transformMatrix__
#define __engine_math_transformMatrix__


#include "engine/math/quaternion.h"

#include "engine/utils/coords.h"




class TransformMatrix
{
public:
	float _matrix[4][4] = {};




public:
	TransformMatrix();
	TransformMatrix(const TransformMatrix& matrix);
	TransformMatrix(float matrix[16]);
	~TransformMatrix() = default;


public:
	float determinant() const;


public:
	const TransformMatrix& calculateTransformMatrix(const Coords3f& translation, const Quaternion& rotation, const Coords3f& scale);

public:
	TransformMatrix& setIdentityMatrix();
	TransformMatrix& setTranslationMatrix(const Coords3f& translation);
	TransformMatrix& setRotationMatrix(const Quaternion& rotation);
	TransformMatrix& setScaleMatrix(const Coords3f& scale);


public:
	TransformMatrix& transpose();


public:
	inline const float* operator[](size_t index) const { return (const float*)_matrix[index]; }
	inline float* operator[](size_t index) { return (float*)_matrix[index]; }
	inline operator const float* () const { return (const float*)_matrix; }


public:
	friend TransformMatrix operator*(const TransformMatrix& m1, const TransformMatrix& m2);
	friend Coords4f operator*(const TransformMatrix& m, const Coords4f& p);

};


#endif // !__engine_math_matrix4f__
