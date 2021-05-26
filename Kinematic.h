#pragma once
#include "Object.h"
#include "Physical.h"

class Kinematic : public Physical
{
public:
	Kinematic(float x, float y, float size, float deg,
		const std::vector<Hitbox>& thitbox = {}) : Physical(x, y, size, deg, thitbox) 
	{
		register_as(TYPE::KINEMATIC);
	}
protected:
	Kinematic& operator=(const Kinematic& other) {
		Physical::operator=(other);
	}
	virtual bool solid(const vec2d extra_data = { 0.0, 0.0 }) override { return true; }
};
