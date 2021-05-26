#pragma once
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "Object.h"

struct keybinding {
	const char* description;
	int glfw_keycode;
};

class Input;

enum MOUSE_USAGE : uint8_t
{
	USE_MOUSE_MOVEMENT = 0x01,
	USE_MOUSE_BUTTONS = 0x02,
	USE_MOUSE_SCROLL = 0x04,
};

// vector safe
class Controlled : virtual public Object
{
public:
	virtual ~Controlled() noexcept {} // needet for moving if stored in vector

	Controlled(std::initializer_list<keybinding> inkeys);
	Controlled(uint8_t mmap, const std::vector<keybinding>& inkeys = {});

	void set_mouse_map(uint8_t mmap) { if (mmap > 7) { mmap = 7; } mouse_map = mmap; }
	void rebind(const char* description, int glfw_keycode_to);
	void disable_input() {
		enabled = false;
		for (int i = 0; i < keys.size(); i++)
		{
			if (arr_controlled[keys[i] - offset] == this)
			{
				arr_controlled[keys[i] - offset] = nullptr;
			}
		}
		for (int i = 0, j = mouse_map; j > 0; i++, j >>= 1)
		{
			if (arr_mouse[i] == this)
			{
				arr_mouse[i] =  nullptr;
			}
		}
	}
	void enable_input() {
		enabled = true;
		for (int i = 0; i < keys.size(); i++)
		{
			if (arr_controlled[keys[i] - offset] == nullptr)
			{
				arr_controlled[keys[i] - offset] = this;
			}
		}
		for (int i = 0, j = mouse_map; j > 0; i++, j >>= 1)
		{
			if (j | 0x01 && arr_mouse[i] == nullptr)
			{
				arr_mouse[i] = this;
			}
		}
	}
	void enable_input_override() noexcept // worst case it segfaults if i fucked something up because of writing straight to memory pointers i love em
	{
		enabled = true;
		for (int i = 0; i < keys.size(); i++)
		{
			arr_controlled[keys[i] - offset] = this;
		}
		for (int i = 0, j = mouse_map; j > 0; i++, j >>= 1)
		{
			if (j | 0x01)
			{
				arr_mouse[i] = this;
			}
		}
	}
	virtual std::string get_description() const;
	virtual void on_key(int key, int scancode, int action, int mods) { return; };
	virtual void on_mouse_move(double xpos, double ypos) { return; };
	virtual void on_mouse_scroll(double xoffset, double yoffset) { return; };
	virtual void on_mouse_click(int button, int action, int mods) { return; };
protected:
	Controlled& operator=(const Controlled& other);
	Controlled& operator=(Controlled&& other) noexcept;
	Controlled(const Controlled& other);
	Controlled(Controlled&& other) noexcept;
private:
	friend class Input;
	static Controlled** arr_controlled;
	static Controlled** arr_mouse;
	static int offset;

	bool enabled = false;
	uint8_t mouse_map = 0x0;
	std::unordered_map<std::string, int> descriptions;
	std::vector<int> keys;
};
