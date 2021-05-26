#pragma once
#include "Object.h"

// Vector Safe
class Positional : virtual public Object
{
public:
	Positional(float x = 0.0f, float y = 0.0f) : x(x), y(y) 
	{
		register_as(TYPE::POSITIONAL);
	}
	virtual void set_x(float nx) { this->x = nx; }
	float get_x() const { return x; }
	virtual void set_y(float ny) { this->y = ny; }
	float get_y() const { return y; }
	virtual void move(float dx, float dy) { this->set_x(x + dx); this->set_y(y + dy); }
	virtual void place(float nx, float ny) { this->set_x(nx); this->set_y(ny); }
protected:
	Positional& operator=(const Positional& other) {
		if (this != &other)
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}
	float x = 0.0f, y = 0.0f;
};
