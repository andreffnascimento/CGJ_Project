#ifndef __UTILS_COORDS_H__
#define __UTILS_COORDS_H__


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

	friend bool operator==(const Coords3f &c1, const Coords3f &c2) { return c1.x == c2.x && c1.y == c2.y && c1.z == c2.z; }
	friend bool operator!=(const Coords3f &c1, const Coords3f &c2) { return c1.x != c2.x || c1.y != c2.y || c1.z != c2.z; }
};


struct SphericalCoords
{
	float alpha;
	float beta;
	float r;
};


#endif // !__UTILS_COORDS_H__
