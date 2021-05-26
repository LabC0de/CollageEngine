#include "Input.h"
#include <GLFW/glfw3.h>

int Input::min_scancode = INT_MAX;
std::vector<Controlled*> Input::on_keys = {};
Controlled* Input::on_mouse[3] = { nullptr, nullptr, nullptr };
int Controlled::offset = 0;
Controlled** Controlled::arr_controlled = nullptr;
Controlled** Controlled::arr_mouse = nullptr;

bool Input::check_key_pressed(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

void Input::set_cursor(uint8_t* rgbapixels, int width, int height)
{
	GLFWimage image;
	image.height = height;
	image.width = width;
	image.pixels = rgbapixels;
	GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
	glfwSetCursor(window, cursor);
}

Input::Input(GLFWwindow* wnd) : window(wnd)
{
	if (on_keys.empty())
	{
		int max_scancode = INT_MIN;
		for (int i = 32; i < GLFW_KEY_LAST; i++)
		{
			int scancode = glfwGetKeyScancode(i);
			if (scancode != -1)
			{
				if (scancode < min_scancode)
				{
					min_scancode = scancode;
				}
				if (scancode > max_scancode)
				{
					max_scancode = scancode;
				}
			}
		}
		on_keys.assign(max_scancode - min_scancode, nullptr);
		Controlled::arr_controlled = on_keys.data();
		Controlled::arr_mouse = on_mouse;
		Controlled::offset = min_scancode;
		glfwSetKeyCallback(window, Input::key_callback);

		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
	}
}
