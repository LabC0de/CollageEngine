#pragma once
#include "AABB.h"
#include <vector>

class Hitbox
{
public:
	Hitbox(std::initializer_list<vec2d> in)
	{
		if (in.size() == 0)
		{
			hitbox = {
				{-1, 0},
				{ 1, 0},
				{ 1, 2},
				{-1, 2}
			};
			centroid = { 0, 1.0f };
		}
		else
		{
			hitbox.assign(in);
			float twicea = 0;
			centroid = { 0 , 0 };
			for (int i = 0; i < hitbox.size(); i++)
			{
				float cross = (hitbox[i].x * hitbox[(i + 1) % hitbox.size()].y) - (hitbox[(i + 1) % hitbox.size()].x * hitbox[i].y);
				twicea += cross;
				centroid.x += (hitbox[i].x + hitbox[(i + 1) % hitbox.size()].x) * cross;
				centroid.y += (hitbox[i].y + hitbox[(i + 1) % hitbox.size()].y) * cross;
			}
			centroid.x /= (3 * twicea);
			centroid.y /= (3 * twicea);
		}
		cp.resize(hitbox.size());
		update(0.0, 0.0, 0.0, 1.0);
	}
	bool test_point(float x, float y) const
	{
		size_t cpsz = cp.size();

		vec2d a = cp[0];
		vec2d b = cp[1];
		float sgn1 = (y - a.y) * (b.x - a.x) - (x - a.x) * (b.y - a.y);
		if (sgn1 == 0) return true;

		for (int i = 1; i < cpsz; i++)
		{
			a = cp[i];
			b = cp[(i + 1) % cpsz];
			float tmp = (y - a.y) * (b.x - a.x) - (x - a.x) * (b.y - a.y);
			if (tmp == 0) return true;
			if ((sgn1 < 0) != (tmp < 0)) return false;
		}
		return true;
	}
	vec2d hits(const Hitbox& other) const
	{
		const Hitbox* a = this;
		const Hitbox* b = &other;
		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				b = this;
				a = &other;
			}
			vec2d line1_start = a->center;
			for (int i = 0; i < a->cp.size(); i++)
			{
				vec2d line1_end = a->cp[i];
				size_t bcpsz = b->cp.size();
				vec2d displacement = { 0, 0 };
				for (int j = 0; j < bcpsz; j++)
				{
					vec2d line2_start = b->cp[j];
					vec2d line2_end = b->cp[(j + 1) % bcpsz];
					float h = (line2_end.x - line2_start.x) * (line1_start.y - line1_end.y) - (line1_start.x - line1_end.x) * (line2_end.y - line2_start.y);
					float t1 = ((line2_start.y - line2_end.y) * (line1_start.x - line2_start.x) + (line2_end.x - line2_start.x) * (line1_start.y - line2_start.y)) / h;
					float t2 = ((line1_start.y - line1_end.y) * (line1_start.x - line2_start.x) + (line1_end.x - line1_start.x) * (line1_start.y - line2_start.y)) / h;

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						displacement.x += (1.0f - t1) * (line1_end.x - line1_start.x);
						displacement.y += (1.0f - t1) * (line1_end.y - line1_start.y);
					}
				}
				if (displacement.x != 0 && displacement.y != 0)
				{
					return shape == 0 ? -displacement : displacement;
				}
			}
		}
		return {0.0, 0.0};
	}
	void update(float ax, float ay,float deg, float sz, float ratio = 1.0f)
	{

		for (int i = 0; i < hitbox.size(); i++)
		{
			cp[i] = (hitbox[i] * sz).rotate(deg).add(ax, ay);
		}
		center = (centroid * sz).rotate(deg).add(ax, ay);
	}
private:
	std::vector<vec2d> cp;
	std::vector<vec2d> hitbox;
	vec2d center, centroid;
};
