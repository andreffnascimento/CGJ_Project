#ifndef __engine_physics_force__
#define __engine_physics_force__


#include "engine/utils/coords.h"




class Force
{

public:
	enum class ForceType
	{
		LINEAR,
		ANGULAR,
	};




private:
	ForceType _type = ForceType::LINEAR;
	Coords3f _value = Coords3f();




public:
	Force() = default;
	Force(const Force&) = default;
	Force(const ForceType& type, const Coords3f& value);
	~Force() = default;


public:
	inline const Force::ForceType& type() const { return _type; }
	inline const Coords3f& value() const		{ return _value; }

};


#endif // !__engine_physics_force__
