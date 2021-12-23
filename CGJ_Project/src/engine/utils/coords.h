#ifndef __engine_utils_coords__
#define __engine_utils_coords__


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

	void operator+=(const Coords3f& other) { x += other.x; y += other.y; z += other.z; }
	void operator-=(const Coords3f& other) { x -= other.x; y -= other.y; z -= other.z; }
	void operator*=(const Coords3f& other) { x *= other.x; y *= other.y; z *= other.z; }
	void operator/=(const Coords3f& other) { x /= other.x; y /= other.y; z /= other.z; }

	bool operator==(const Coords3f &other) const { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const Coords3f &other) const { return x != other.x || y != other.y || z != other.z; }

	friend Coords3f operator+(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x + c2.x, c1.y + c2.y, c1.z + c2.z }); }
	friend Coords3f operator-(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x - c2.x, c1.y - c2.y, c1.z - c2.z }); }
	friend Coords3f operator*(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x * c2.x, c1.y * c2.y, c1.z * c2.z }); }
	friend Coords3f operator/(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x / c2.x, c1.y / c2.y, c1.z / c2.z }); }
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

	void operator+=(const Coords4f& other) { x += other.x; y += other.y; z += other.z; w += other.w; }
	void operator-=(const Coords4f& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; }
	void operator*=(const Coords4f& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; }
	void operator/=(const Coords4f& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; }

	bool operator==(const Coords4f& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
	bool operator!=(const Coords4f& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

	friend Coords4f operator+(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x + c2.x, c1.y + c2.y, c1.z + c2.z, c1.w + c2.w }); }
	friend Coords4f operator-(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x - c2.x, c1.y - c2.y, c1.z - c2.z, c1.w - c2.w }); }
	friend Coords4f operator*(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x * c2.x, c1.y * c2.y, c1.z * c2.z, c1.w * c2.w }); }
	friend Coords4f operator/(const Coords4f& c1, const Coords4f c2) { return Coords4f({ c1.x / c2.x, c1.y / c2.y, c1.z / c2.z, c1.w / c2.w }); }
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

	bool operator==(const SphericalCoords& other) const { return alpha == other.alpha && beta == other.beta && r == other.r; }
	bool operator!=(const SphericalCoords& other) const { return alpha != other.alpha && beta != other.beta && r != other.r; }
};


#endif // !__engine_utils_coords__