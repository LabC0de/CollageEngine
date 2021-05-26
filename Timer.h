#pragma once
#include "Timed.h"
#include <chrono>
#include <deque>
#include <algorithm>

class Timer
{
public:
	static float clock()
	{
		
		delta = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count();
		start = std::chrono::steady_clock::now();
		for (Timed* obj : timed_objects)
		{
			obj->timed_update(delta);
		}
		return delta;
	}
	static void reset()
	{
		timed_objects.clear();
	}
	static void init()
	{
		start = std::chrono::steady_clock::now();
	}
	static void make_entry(Timed& obj)
	{
		timed_objects.push_back(&obj);
		obj.time_handler = &timed_objects;
	}
	static void remove_entry(Timed& obj)
	{
		auto end = timed_objects.end();
		if (end != timed_objects.erase(std::remove(timed_objects.begin(), timed_objects.end(), &obj))) {
			obj.time_handler = nullptr;
		}
	}
private:
	static std::deque<Timed*> timed_objects;
	static std::chrono::steady_clock::time_point start;
	friend class Timed;
	static float delta;
};