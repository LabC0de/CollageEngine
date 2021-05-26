#pragma once

#include <cstdio>
#include "Layer.h"

typedef struct GLFWwindow GLFWwindow;

class Renderer
{
public:
	void clear_color() const;
	void draw();
	void reset()
	{
		standard_layers.clear();
	}
	void add_layer(DynamicLayer& obj)
	{
		standard_layers.push_back(&obj);
	}
	void swap_buffer() const;
	bool is_initialized() const { return window != nullptr; }
private:
	friend class Engine;
	// has to be a layerchain
	std::vector<DynamicLayer*> standard_layers;
	Renderer() : window(nullptr) {}
	Renderer(GLFWwindow* window);
	GLFWwindow* window = nullptr;
};