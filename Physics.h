#pragma once
#include <vector>
#include <thread>
#include <algorithm>
#include "Physical.h"
#include "Triggerbox.h"

class Physics
{
public:
	Physical* probe(float x, float y, Object* optional = nullptr)
	{
		Physical* ret = nullptr;
		size_t i = 0;
		size_t sz = objects.size();
		// assume sorted array before probing wich is likely since the physicts test is the very first step of every tick
		size_t low = sz / 2;
		for (; objects[low]->get_x() > x - maxszx; low /= 2)
		{
			if (low == 0) break;
		}
		size_t top = sz / 2;
		for (; objects[low]->get_x() < x + maxszx; top += ullmax(((sz - top) / 2), 1))
		{
			if (top >= sz) {
				top = sz;
				break;
			}
		}
		i = low;
		sz = top;
		float rety = 0.0f;
		for (; i < sz; i++) {
			if (objects[i]->check_point(x, y)) {
				ret = objects[i];
				rety = ret->get_y();
				break;
			}
		}
		for (; i < sz; i++) {
			if (objects[i]->check_point(x, y)) {
				if (objects[i]->get_y() < rety) {
					ret = objects[i];
				}
			}
		}
		if (ret) ret->on_probe(optional);
		return ret;
	}
	void make_entry(Physical& obj)
	{
		obj.phys_handler = this;
		objects.push_back(&obj);
	}
	void remove_entry(Physical& obj)
	{
		auto end = objects.end();
		if (objects.erase(std::remove(objects.begin(), objects.end(), &obj)) != end) {
			obj.phys_handler = nullptr;
		}
	}
	void reset()
	{
		for (auto obj : objects) {
			obj->phys_handler = nullptr;
		}
		objects.clear();
	}
	void disable() 
	{ 
		mbenabled = false; 
	}
	void enable() 
	{
		mbenabled = true; 
	}
	void calculate()
	{
		if (!mbenabled) return;
		// insertion sort for almost sorted data
		float max_sz = 0.0;
		size_t end = objects.size();
		for (size_t i = 1; i < end; ++i)
		{
			Physical* to_sort = objects[i];
			float tmp = check_size_changes(to_sort);
			max_sz = tmp > max_sz ? tmp : max_sz;
			float sorting_x = to_sort->get_x();

			size_t j = i;
			for (; sorting_x < objects[j - 1]->get_x() && j > 0; --j)
			{
				objects[j] = objects[j - 1];
			}
			objects[j] = to_sort;
		}
		for (size_t i = 0; i < end; ++i)
		{
			if (!objects[i]->active) { 
				continue; 
			}
			float ax = objects[i]->get_x() + max_sz;
			objects[i]->on_physics_test();
			bool fstatic = objects[i]->is_static();
			for (size_t j = i + 1; j < end; ++j)
			{
				if (objects[j]->is_static() && fstatic) { 
					continue; 
				}
				float bx = objects[j]->get_x() - max_sz;
				if (ax < bx) break;
				if (objects[i]->probeaabb(*objects[j]))
				{
					objects[i]->on_aabb_collision(*objects[j]);
					objects[j]->on_aabb_collision(*objects[i]);
					if (vec2d offset = objects[i]->calculate_collision(*objects[j]))
					{
						objects[i]->resolve_collision(offset, *objects[j]);
						objects[j]->resolve_collision(-offset, *objects[i]);
					}
				}
			}
			objects[i]->after_physics_test();
		}
		maxszx = max_sz;
	}
private:
	size_t ullmax(size_t a, size_t b) {
		return a > b ? a : b;
	}
	// x1 is always <= x2
	float distance(float x1, float x2)
	{
		if (x1 >= 0) return x2 - x1;
		if (x2 <= 0) return fabsf(x1 - x2);
		return fabsf(x1) + x2;
	}
	float check_size_changes(const Physical* obj)
	{
		float a = 2 * obj->get_size();
		float b = obj->get_ratio() * obj->get_size();
		return sqrtf((a * a) + (b * b));
	}
	std::vector<Physical*> objects;
	bool mbenabled = true;
	float maxszx = 0.0;
};