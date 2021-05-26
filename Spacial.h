#pragma once
#include "Object.h"

// Vector Safe
class Spacial : virtual public Object
{
public:
	Spacial(float size = 1.0, float ratio = 1.0) : size(size), ratio(ratio) 
	{
		register_as(TYPE::SPACIAL);
	}
	virtual void set_size(float nz) { size = nz; }
	virtual void set_ratio(float nr) { ratio = nr; }
	virtual void scale(float by) { set_size(this->get_size() * by); }
	inline float get_size() const { return size; }
	inline float get_ratio() const { return ratio; }
private:
	Spacial& operator=(const Spacial& other) {
		if (this != &other)
		{
			size = other.size;
			ratio = other.ratio;
		}
		return *this;
	}
	float size = 1.0f, ratio = 1.0f;
};
