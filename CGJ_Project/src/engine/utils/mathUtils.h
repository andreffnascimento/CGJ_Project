#ifndef __engine_math_mathUtils__
#define __engine_math_mathUtils__


#include <numeric>




static constexpr double PI = 3.14159265358979323846;




inline float toRadians(float degrees) 
{ 
	return (float)(degrees * (PI / 180.0)); 
}

inline float toDegrees(float radians)
{
	return (float)(radians * (180.0 / PI));
}




float invSqrt(float number);


#endif // !__engine_math_mathUtils__
