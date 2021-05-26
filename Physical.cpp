#include "Physical.h"
#include "Physics.h"

/*
Physical::Physical(const Physical& other) 
	: phys_handler(nullptr), hitbox(other.hitbox), active_hb(other.active_hb), up_to_date(false)
{
	register_as(TYPE::PHYSICAL);
	update_aabb();
}

Physical::Physical(Physical&& other) noexcept
	: phys_handler(nullptr), hitbox(std::move(other.hitbox)), active_hb(other.active_hb), up_to_date(false)
{
	if (other.phys_handler) {
		phys_handler = other.phys_handler;
		phys_handler->remove_entry(other);
		phys_handler->make_entry(*this);
	}
	register_as(TYPE::PHYSICAL);
	update_aabb();
}

Physical& Physical::operator=(const Physical& other)
{
	if (this != &other)
	{
		if (phys_handler) {
			phys_handler->remove_entry(*this);
		}
		hitbox = other.hitbox;
		active_hb = other.active_hb;
		up_to_date = other.up_to_date;
		update_aabb();
	}
	return *this;
}

Physical& Physical::operator=(Physical&& other) noexcept
{
	if (this != &other)
	{
		if (phys_handler) {
			phys_handler->remove_entry(*this);
		}
	}
	return *this;
}
*/

Physical& Physical::operator=(const Physical& other)
{
	if (this != &other)
	{
		if (phys_handler) {
			phys_handler->remove_entry(*this);
		}
		hitbox = other.hitbox;
		active_hb = other.active_hb;
		up_to_date = other.up_to_date;
		update_aabb();
	}
	return *this;
}
