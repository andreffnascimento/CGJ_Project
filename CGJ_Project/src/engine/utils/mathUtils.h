#ifndef __engine_math_mathUtils__
#define __engine_math_mathUtils__




static constexpr double HALF_PI = 1.5707963267948966192313216916397514420985846996875529104874722961;
static constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;
static constexpr double TWO_PI = 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696;




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
