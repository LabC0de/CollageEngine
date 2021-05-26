#pragma once
#include "Controlled.h"

typedef struct GLFWwindow GLFWwindow;

class Input
{
public:
	bool check_key_pressed(int key);
	bool is_initialized()
	{
		return window != nullptr;
	}
	static void make_entry(Controlled& pcontrolled)
	{
		for (int binding : pcontrolled.keys)
		{
			on_keys[binding - min_scancode] = &pcontrolled;
		}
		if(pcontrolled.mouse_map & 0x01) on_mouse[0] = &pcontrolled;
		if(pcontrolled.mouse_map & 0x02) on_mouse[1] = &pcontrolled;
		if(pcontrolled.mouse_map & 0x04) on_mouse[2] = &pcontrolled;
	}
	static void remove_entry(Controlled& pcontrolled)
	{
		for (int binding : pcontrolled.keys)
		{
			if (on_keys[binding - min_scancode] == &pcontrolled) // if a controll got overridden by another controlled don't fuck them over
			{
				on_keys[binding - min_scancode] = nullptr;
			}
		}
		if (pcontrolled.mouse_map & 0x01 && on_mouse[0] == &pcontrolled) on_mouse[0] = nullptr;
		if (pcontrolled.mouse_map & 0x02 && on_mouse[0] == &pcontrolled) on_mouse[1] = nullptr;
		if (pcontrolled.mouse_map & 0x04 && on_mouse[0] == &pcontrolled) on_mouse[2] = nullptr;
	}
	static void reset() {
		for (size_t i = 0; i < on_keys.size(); i++)
			on_keys[i] = nullptr;
		for (size_t i = 0; i < 3; i++)
			on_mouse[i] = nullptr;
	}
	void set_cursor(uint8_t* rgbapixels, int width, int height);
private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Controlled* obj = on_keys[scancode - min_scancode];
		if (obj)
		{
			obj->on_key(key, scancode, action, mods);
		}
	}
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if(on_mouse[0]) on_mouse[0]->on_mouse_move(xpos, ypos);
	}
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (on_mouse[1]) on_mouse[1]->on_mouse_click(button, action, mods);
	}
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (on_mouse[2]) on_mouse[2]->on_mouse_scroll(xoffset, yoffset);
	}

	friend class Engine;
	Input() = default;
	Input(GLFWwindow* wnd);
	GLFWwindow* window = nullptr;
	static int min_scancode;
	static std::vector<Controlled*> on_keys;
	static Controlled* on_mouse[3];
};
