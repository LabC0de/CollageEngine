#pragma once
#include "Object.h"

class Timed : virtual public Object
{
public:
	Timed()
	{
		register_as(TYPE::TIMED);
	}
	float delta_t() const
	{
		return delta;
	}
	virtual void timed_update(float dt) { return; }
protected:
	Timed& operator=(const Timed& rhs);
private:
	friend class Timer;
	void* time_handler = nullptr; // only for id
	static float& delta;
};