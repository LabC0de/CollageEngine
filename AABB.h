#pragma once
#include "vec2d.h"
#include <math.h>


class AABB
{
public:
	AABB(float x = 0.0, float y = 0.0, float deg = 0.0, float sz = 1.0, float ratio = 1.0)
	{
		update(x, y, deg, sz, ratio);
	}
	bool probeX(float vx) const
	{
		return (vx > l) && (vx < r);
	}
	bool probeY(float vy) const
	{
		return (vy > b) && (vy < t);
	}
	bool probe(float vx, float vy) const
	{
		return probeX(vx) && probeY(vy);
	}
	bool collides(AABB other) const
	{
		return ((b <= other.t) && (t >= other.b) && (l <= other.r) && (r >= other.l));
	}
	void update(float x, float y, float deg, float sz, float ratio = 1.0f)
	{
		r = x; l = x; t = y; b = y;
		vec2d origin = { x, y };
		vec2d offsetx = { (ratio * sz) , 0.0f };
		vec2d offsety = { 0.0f , 2 * sz };
		vec2d corners[4] = { origin + (-offsetx).rotate(deg),
							 origin + (offsetx).rotate(deg),
							 origin + (offsetx + offsety).rotate(deg),
							 origin + (-offsetx + offsety).rotate(deg) };
		for (int i = 0; i < 4; i++)
		{
			l = corners[i].x < l ? corners[i].x : l;
			r = corners[i].x > r ? corners[i].x : r;
			t = corners[i].y > t ? corners[i].y : t;
			b = corners[i].y < b ? corners[i].y : b;
		}
	}
	void move(float x, float y)
	{
		t += y;
		b += y;
		l += x;
		r += x;
	}
private:
	float t, b, l, r;
};