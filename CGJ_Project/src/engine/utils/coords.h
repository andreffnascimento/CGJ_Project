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
};


struct SphericalCoords
{
	float alpha;
	float beta;
	float r;
};


#endif // !__UTILS_COORDS_H__
