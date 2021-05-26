#include "Controlled.h"
#include "Debug.h"
#include <GLFW/glfw3.h>



/*
Controlled& Controlled::operator=(const Controlled& other)
{
	if (this != &other)
	{
		// about to react to new instructions so clean up after yourself
		// doesnt reanable itself because if the buttons are completely different it would be a fucking mess
		// the new object would imeadatly react to a new set of buttons but only if they arent mapped already
		if (enabled) {
			disable_input();
		}

		Object::operator=(other);
		mouse_map = other.mouse_map;
		descriptions = other.descriptions;
		keys = other.keys;
		DBG(printf("Copie Assigned Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}

Controlled& Controlled::operator=(Controlled&& other) noexcept
{
	if (this != &other)
	{
		if (enabled) disable_input();
		bool other_was_enabled = other.enabled;

		other.disable_input();
		Object::operator=(other);
		enabled = true;
		mouse_map = other.mouse_map;
		descriptions = std::move(other.descriptions);
		keys = std::move(other.keys);

		if (other_was_enabled) enable_input();

		DBG(printf("Move Assigned Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}

Controlled::Controlled(const Controlled& other)
	: keys(other.keys), descriptions(other.descriptions), mouse_map(other.mouse_map), enabled(false)
{
	register_as(TYPE::CONTROLLED);
	DBG(printf("Copy Constructed Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
}

Controlled::Controlled(Controlled&& other) noexcept
	: keys(std::move(other.keys)), descriptions(std::move(other.descriptions)), mouse_map(other.mouse_map), enabled(false)
{
	other.disable_input();
	register_as(TYPE::CONTROLLED);
	DBG(printf("Move Constructed Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	enable_input();
}
*/

Controlled::Controlled(std::initializer_list<keybinding> inkeys) {
	register_as(TYPE::CONTROLLED);
	keys.reserve(inkeys.size());
	for (auto binding : inkeys)
	{
		descriptions[binding.description] = (int)keys.size();
		keys.emplace_back(glfwGetKeyScancode(binding.glfw_keycode));
	}
}

Controlled::Controlled(uint8_t mmap, const std::vector<keybinding>& inkeys) : mouse_map(mmap)
{
	register_as(TYPE::CONTROLLED);
	keys.reserve(inkeys.size());
	for (auto binding : inkeys)
	{
		descriptions[binding.description] = (int)keys.size();
		keys.emplace_back(glfwGetKeyScancode(binding.glfw_keycode));
	}
}

void Controlled::rebind(const char* description, int glfw_keycode_to)
{
	auto tmp = descriptions.find(description);
	if (tmp != descriptions.end())
	{
		if (arr_controlled[keys[tmp->second] - offset] == this)
		{
			arr_controlled[keys[tmp->second] - offset] = nullptr;
		}
		keys[tmp->second] = glfwGetKeyScancode(glfw_keycode_to);
		arr_controlled[keys[tmp->second] - offset] = this;

	}
}

std::string Controlled::get_description() const
{
	std::stringstream ss;
	for (const auto& mapping : descriptions)
	{
		ss << mapping.first << " " << glfwGetKeyName(GLFW_KEY_UNKNOWN, keys[mapping.second]) << std::endl;
	}
	return ss.str();
}

Controlled& Controlled::operator=(const Controlled& other)
{
	if (this != &other)
	{
		// about to react to new instructions so clean up after yourself
		// doesnt reanable itself because if the buttons are completely different it would be a fucking mess
		// the new object would imeadatly react to a new set of buttons but only if they arent mapped already
		if (enabled) {
			disable_input();
		}
		mouse_map = other.mouse_map;
		descriptions = other.descriptions;
		keys = other.keys;
		DBG(printf("Copie Assigned Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}

Controlled& Controlled::operator=(Controlled&& other) noexcept
{
	if (this != &other)
	{
		if (enabled) disable_input();
		bool other_was_enabled = other.enabled;

		other.disable_input();
		enabled = true;
		mouse_map = other.mouse_map;
		descriptions = std::move(other.descriptions);
		keys = std::move(other.keys);

		if (other_was_enabled) enable_input();

		DBG(printf("Move Assigned Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}

Controlled::Controlled(const Controlled& other)
	: keys(other.keys), descriptions(other.descriptions), mouse_map(other.mouse_map), enabled(false)
{
	register_as(TYPE::CONTROLLED);
	DBG(printf("Copy Constructed Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
}

Controlled::Controlled(Controlled&& other) noexcept
	: keys(std::move(other.keys)), descriptions(std::move(other.descriptions)), mouse_map(other.mouse_map), enabled(false)
{
	bool other_was_enabled = other.enabled;
	other.disable_input();
	register_as(TYPE::CONTROLLED);
	if (other_was_enabled) enable_input();
	DBG(printf("Move Constructed Controlled: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
		enable_input();
}

