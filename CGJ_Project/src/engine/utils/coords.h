#ifndef __engine_utils_coords__
#define __engine_utils_coords__


#include <cmath>

#include "engine/utils/mathUtils.h"




struct WindowCoords
{
	unsigned int x = 0;
	unsigned int y = 0;

	WindowCoords() = default;
	WindowCoords(const WindowCoords&) = default;
	~WindowCoords() = default;
};


struct Coords3f
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Coords3f() = default;
	Coords3f(const Coords3f&) = default;
	~Coords3f() = default;

	float length() const					{ return std::sqrt(length2()); }
	float length2() const					{ return x * x + y * y + z * z; }
	float dot(const Coords3f& other) const	{ return x * other.x + y * other.y + z * other.z; }
	Coords3f& normalize()					{ float invNorm = invSqrt(length2()); x *= invNorm; y *= invNorm; z *= invNorm; return *this; }

	Coords3f normalized() const				{ float invNorm = invSqrt(length2()); return Coords3f({ x * invNorm, y * invNorm, z * invNorm }); }

	void operator+=(const Coords3f& other) { x += other.x; y += other.y; z += other.z; }
	void operator-=(const Coords3f& other) { x -= other.x; y -= other.y; z -= other.z; }
	void operator*=(const Coords3f& other) { x *= other.x; y *= other.y; z *= other.z; }
	void operator/=(const Coords3f& other) { x /= other.x; y /= other.y; z /= other.z; }

	void operator*=(float value) { x *= value; y *= value; z *= value; }
	void operator/=(float value) { x /= value; y /= value; z /= value; }

	bool operator==(const Coords3f &other) const { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const Coords3f &other) const { return x != other.x || y != other.y || z != other.z; }

	friend Coords3f operator-(const Coords3f& c) { return Coords3f({ -c.x, -c.y, -c.z }); }

	friend Coords3f operator+(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x + c2.x, c1.y + c2.y, c1.z + c2.z }); }
	friend Coords3f operator-(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x - c2.x, c1.y - c2.y, c1.z - c2.z }); }
	friend Coords3f operator*(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x * c2.x, c1.y * c2.y, c1.z * c2.z }); }
	friend Coords3f operator/(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x / c2.x, c1.y / c2.y, c1.z / c2.z }); }

	friend Coords3f operator*(const Coords3f& c1, float value) { return Coords3f({ c1.x * value, c1.y * value, c1.z * value }); }
	friend Coords3f operator/(const Coords3f& c1, float value) { return Coords3f({ c1.x / value, c1.y / value, c1.z / value }); }
	friend Coords3f operator*(float value, const Coords3f& c1) { return Coords3f({ c1.x * value, c1.y * value, c1.z * value }); }
	friend Coords3f operator/(float value, const Coords3f& c1) { return Coords3f({ c1.x / value, c1.y / value, c1.z / value }); }
};


struct Coords4f
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	Coords4f() = default;
	Coords4f(const Coords4f&) = default;
	~Coords4f() = default;

	float length() const					{ return std::sqrt(length2()); }
	float length2() const					{ return x * x + y * y + z * z + w * w; }
	float dot(const Coords4f& other) const	{ return x * other.x + y * other.y + z * other.z + w * other.w; }
	Coords4f& normalize()					{ float invNorm = invSqrt(length2()); x *= invNorm; y *= invNorm; z *= invNorm; w *= invNorm; return *this; }

	Coords4f normalized() const				{ float invNorm = invSqrt(length2()); return Coords4f({ x * invNorm, y * invNorm, z * invNorm, w * invNorm }); }

	void operator+=(const Coords4f& other) { x += other.x; y += other.y; z += other.z; w += other.w; }
	void operator-=(const Coords4f& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; }
	void operator*=(const Coords4f& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; }
	void operator/=(const Coords4f& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; }

	bool operator==(const Coords4f& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
	bool operator!=(const Coords4f& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

	void operator*=(float value) { x *= value; y *= value; z *= value; w *= value; }
	void operator/=(float value) { x /= value; y /= value; z /= value; w /= value; }

	friend Coords4f operator-(const Coords4f& c) { return Coords4f({ -c.x, -c.y, -c.z, -c.w }); }

	friend Coords4f operator+(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x + c2.x, c1.y + c2.y, c1.z + c2.z, c1.w + c2.w }); }
	friend Coords4f operator-(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x - c2.x, c1.y - c2.y, c1.z - c2.z, c1.w - c2.w }); }
	friend Coords4f operator*(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x * c2.x, c1.y * c2.y, c1.z * c2.z, c1.w * c2.w }); }
	friend Coords4f operator/(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x / c2.x, c1.y / c2.y, c1.z / c2.z, c1.w / c2.w }); }

	friend Coords4f operator*(const Coords4f& c1, float value) { return Coords4f({ c1.x * value, c1.y * value, c1.z * value, c1.w * value }); }
	friend Coords4f operator/(const Coords4f& c1, float value) { return Coords4f({ c1.x / value, c1.y / value, c1.z / value, c1.w / value }); }
	friend Coords4f operator*(float value, const Coords4f& c1) { return Coords4f({ c1.x * value, c1.y * value, c1.z * value, c1.w * value }); }
	friend Coords4f operator/(float value, const Coords4f& c1) { return Coords4f({ c1.x / value, c1.y / value, c1.z / value, c1.w / value }); }
};




struct ClippingPlanes
{
	float near = -1.0f;
	float far = 1.0f;

	ClippingPlanes() = default;
	ClippingPlanes(const ClippingPlanes&) = default;
	~ClippingPlanes() = default;

	bool operator==(const ClippingPlanes& other) const { return near == other.near && far == other.far; }
	bool operator!=(const ClippingPlanes& other) const { return near != other.near || far != other.far; }
};


struct Rectf
{
	float left = -1.0f;
	float right = 1.0f;
	float bottom = -1.0f;
	float top = 1.0f;

	Rectf() = default;
	Rectf(const Rectf&) = default;
	~Rectf() = default;

	bool operator==(const Rectf& other) const { return left == other.left && right == other.right && bottom == other.bottom && top == other.top; }
	bool operator!=(const Rectf& other) const { return left != other.left || right != other.right || bottom != other.bottom || top != other.top; }
};




struct SphericalCoords
{
	float alpha = 0.0f;
	float beta = 0.0f;
	float r = 1.0f;

	SphericalCoords() = default;
	SphericalCoords(const SphericalCoords&) = default;
	~SphericalCoords() = default;

	bool operator==(const SphericalCoords& other) const { return alpha == other.alpha && beta == other.beta && r == other.r; }
	bool operator!=(const SphericalCoords& other) const { return alpha != other.alpha && beta != other.beta && r != other.r; }
};




struct ReflectionCoefficients
{
	float ambient = 1.0f;
	float diffuse = 1.0f;
	float specular = 1.0f;

	ReflectionCoefficients() = default;
	ReflectionCoefficients(const ReflectionCoefficients&) = default;
	~ReflectionCoefficients() = default;
};


#endif // !__engine_utils_coords__