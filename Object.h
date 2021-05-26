#pragma once
#include <bitset>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include "Defines.h"

enum TYPE
{
	AUDIBLE,
	CONTROLLED,
	DIRECTIONAL,
	DRAWABLE,
	KINEMATIC,
	NAMED,
	PHYSICAL,
	POSITIONAL,
	SPACIAL,
	STATIC,
	TIMED,
	TRIGGERBOX,
	FIRST_CUSTOM_TYPE
};

class Object
{
public:
	Object() = default;
	bool is_a(int id) const { return typeinfo[id]; }
protected:
	Object& operator=(const Object& other) {
		if (this != &other) typeinfo = other.typeinfo;
		return *this;
	}
	void register_as(int id) { typeinfo[id] = true; }
private:
	Object(const Object& other) {
		typeinfo = other.typeinfo;
	}
	Object(Object&& other) = delete;
	Object& operator=(Object&& other) = delete;

	std::bitset<11 + NO_CUSTOM_OBJECTS> typeinfo = 0;
};