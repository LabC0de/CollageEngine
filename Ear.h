#pragma once
#include <atomic>
#include <cmath>

class Ear
{
public:
	float inv_sqrd_distance_to(float x, float y)
	{
		float a = ax - x;
		float b = ay - y;
		return 1/((a * a) + (b * b));
	}
	void position(float nx, float ny)
	{
		ax = nx;
		ay = ny;
	}
	void move(float dx, float dy)
	{
		ax = ax + dx;
		ay = ay + dy;
	}
private:
	std::atomic<float> ax, ay;
};