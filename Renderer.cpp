#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Debug.h"

void Renderer::clear_color() const
{
	GLC(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw()
{
	for (auto lay : standard_layers)
	{
		if (!lay->is_hidden()) lay->draw();
		// if layer last
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

void Renderer::swap_buffer() const
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

Renderer::Renderer(GLFWwindow* window) : window(window)
{
	GLC(glEnable(GL_BLEND));
	GLC(glEnable(GL_DEPTH_TEST));
	GLC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}