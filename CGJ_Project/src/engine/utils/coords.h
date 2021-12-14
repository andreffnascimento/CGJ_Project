#ifndef __ENGINE_UTILS_COORDS_H__
#define __ENGINE_UTILS_COORDS_H__


struct Coords2i
{
	int x;
	int y;
};


struct Coords3i
{
	int x;
	int y;
	int z;
};


struct Coords3f
{
	float x;
	float y;
	float z;

	void operator+=(const Coords3f& other) { x += other.x; y += other.y; z += other.z; }
	void operator-=(const Coords3f& other) { x -= other.x; y -= other.y; z -= other.z; }

	bool operator==(const Coords3f &other) const { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const Coords3f &other) const { return x != other.x || y != other.y || z != other.z; }

	friend Coords3f operator+(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x + c2.x, c1.y + c2.y, c1.z + c2.z }); }
	friend Coords3f operator-(const Coords3f& c1, const Coords3f c2) { return Coords3f({ c1.x - c2.x, c1.y - c2.y, c1.z - c2.z }); }
};


struct SphericalCoords
{
	float alpha;
	float beta;
	float r;
};


#endif // !__ENGINE_UTILS_COORDS_H__
