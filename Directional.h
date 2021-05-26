#pragma once
#include "Object.h"

// Vector Save
class Directional : virtual public Object
{
public:
	Directional(float deg = 0.0f) : degrees(deg) 
	{
		register_as(TYPE::DIRECTIONAL);
	}
	Directional(const Directional& other) : degrees(other.degrees) {}
	float get_rotation() const { return degrees; }
	virtual void set_rotation(float nd) { degrees = nd; }
	virtual void rotate(float by) 
	{
		float rot = degrees + by;
		rot -= (float(int(rot / 360.0f)) * 360.0f); // maybe if is faster than a div mul...
		this->set_rotation(rot);
	}
protected:
	Directional& operator=(const Directional& other) {
		degrees = other.degrees;
	}
	float degrees = 0.0;
};
