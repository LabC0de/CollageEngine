#pragma once
#include "Physical.h"
#include <unordered_map>

class Triggerbox : public Physical
{
public:
	Triggerbox(float x, float y, float size = 1.0, float deg = 0.0) : Positional(x, y), Spacial(size), Directional(deg), Physical{}
	{
		register_as(TYPE::TRIGGERBOX);
		update_aabb();
	}
	// check if center is in aabb for aditional securety
protected:
	// override on aabb collision and check if collision already occured
	// or override on physics test and use sat
	virtual void resolve_collision(vec2d collision, Physical& other) 
	{ 
		if (touching_objects.find(&other) == touching_objects.end())
			touching_objects[&other] = 2;
		else
			touching_objects[&other] = 1;
	}
	virtual void on_initial_contact(Physical* other)
	{
		return;
	}
	virtual void on_continued_contact(Physical* other)
	{
		return;
	}
	virtual void on_contact_end(Physical* other)
	{
		return;
	}
	virtual void after_physics_test() override {
		for (auto it = touching_objects.begin(); it != touching_objects.end(); ) {
			switch (it->second)
			{
			case 0:
				this->on_contact_end(it->first);
				it = touching_objects.erase(it);
				break;
			case 1:
				it->second = 0;
				this->on_continued_contact(it->first);
				++it;
				break;
			case 2:
				it->second = 1;
				this->on_initial_contact(it->first);
				++it;
				break;
			}
		}
	}
private:
	std::unordered_map<Physical*, char> touching_objects;
};
