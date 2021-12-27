#ifndef __engine_math_force__
#define __engine_math_force__


#include "engine/utils/coords.h"




class Force
{

public:
	enum class ForceType
	{
		LINEAR,
		ANGULAR
	};




private:
	Coords3f _force = Coords3f();
	ForceType _type = ForceType::LINEAR;



public:
	Force() = default;
	Force(const Force&) = default;
	Force(const Coords3f& force, const ForceType& type);
	~Force() = default;


public:
	inline const Coords3f& force() const { return _force; }
	inline const ForceType& type() const { return _type; }
	
};


#endif // !__engine_math_force__