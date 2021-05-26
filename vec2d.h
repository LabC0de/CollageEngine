#pragma once

struct vec2d {
	float x, y;
	vec2d operator-() const
	{
		return { -x, -y };
	}
	vec2d operator*(float scalar) const
	{
		return { x * scalar, y * scalar };
	}
	vec2d operator+(vec2d other) const
	{
		return { x + other.x, y + other.y };
	}
	vec2d operator-(vec2d other) const
	{
		return { x - other.x, y - other.y };
	}
	vec2d add(float ax, float ay) const
	{
		return { x + ax, y + ay };
	}
	operator bool() const { return x != 0.0f && y != 0.0f; }
	vec2d rotate(float degrees) const
	{
		float rad = (degrees * 3.14159262f) / 180.0f;
		float cosa = cosf(rad);
		float sina = sinf(rad);
		return {
			x * cosa - y * sina,
			x * sina + y * cosa
		};
	}
};