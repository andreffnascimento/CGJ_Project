#ifndef __engine_math_transformMatrix__
#define __engine_math_transformMatrix__


#include "engine/utils/coords.h"




class TransformMatrix
{
public:
	float _matrix[4][4] = {};




public:
	TransformMatrix();
	TransformMatrix(const TransformMatrix& matrix);
	~TransformMatrix() = default;


public:
	const TransformMatrix& calculateTransformMatrix(const Coords3f& translation, const Coords3f& rotation, const Coords3f& scale);

public:
	TransformMatrix& setIdentityMatrix();
	TransformMatrix& setTranslationMatrix(const Coords3f& translation);
	TransformMatrix& setRotationMatrix(const Coords3f& rotation);
	TransformMatrix& setScaleMatrix(const Coords3f& scale);


public:
	TransformMatrix& transpose();


public:
	inline const float* operator[](size_t index) const { return (const float*)_matrix[index]; }
	inline float* operator[](size_t index) { return (float*)_matrix[index]; }
	inline operator const float* () const { return (const float*)_matrix; }


public:
	friend TransformMatrix operator*(const TransformMatrix& m1, const TransformMatrix& m2);

};


#endif // !__engine_math_matrix4f__
