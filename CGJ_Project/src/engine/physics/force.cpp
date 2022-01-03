#include "force.h"




Force::Force(const ForceType& type, const Coords3f& value)
	: _type(type), _value(value)
{
	// empty
}