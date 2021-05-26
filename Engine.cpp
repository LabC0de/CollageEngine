#include "Engine.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Engine::Engine(int widh, int height, const char* title) : cool(L""), collision(), width(widh), height(height)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		printf("GLFW Failed to initialize\n");
		throw - 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(widh, height, title, NULL, NULL);
	if (!window)
	{
		printf("GLFW Failed to create window\n");
		glfwTerminate();
		throw - 1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		printf("Glew Failed to initialize\n");
		throw - 1;
	}
	glfwSetWindowUserPointer(window, this);
	collage = Renderer(window);
	collector = Input(window);
}

bool Engine::done()
{
	return glfwWindowShouldClose(window) || b_kill;
}

Engine::~Engine()
{
	// important to clean after yourself before terminating the context
	Texture::flush_pool();
	cool.kill();
	glfwDestroyWindow(window);
	glfwTerminate();
}