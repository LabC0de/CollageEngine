#pragma once
#include "vec2d.h"
#include <vector>

class Border
{
public:
	Border(std::initializer_list<vec2d> points) : line(points) {};
private:
	std::vector<vec2d> line;
};
