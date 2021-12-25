#ifndef __engine_math_angle__
#define __engine_math_angle__


static constexpr double PI = 3.14159265358979323846;




inline float toRadians(float degrees) 
{ 
	return (float)(degrees * (PI / 180.0)); 
}


#endif // !__engine_math_angle__
