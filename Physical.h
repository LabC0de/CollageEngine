#pragma once
#include "Directional.h"
#include "Spacial.h"
#include "Positional.h"
#include "Hitbox.h"
#include <unordered_set>

class Physics;

class Physical : virtual public Directional, virtual public Spacial, virtual public Positional
{
public:
	Physical(std::initializer_list<Hitbox> init) : hitbox(init), active_hb(0), up_to_date(false)
	{
		register_as(TYPE::PHYSICAL);
		if (hitbox.empty())
		{
			hitbox.emplace_back(Hitbox{});
		}
		update_aabb();
	}
	Physical(const std::vector<Hitbox>& thitbox) : hitbox(thitbox), active_hb(0), up_to_date(false)
	{
		register_as(TYPE::PHYSICAL);
		if (hitbox.empty())
		{
			hitbox.emplace_back(Hitbox{});
		}
		update_aabb();
	}
	Physical(float x, float y, float size, float deg, 
		const std::vector<Hitbox>& thitbox = {}) : Positional(x, y), Spacial(size), Directional(deg), hitbox(thitbox), up_to_date(false)
	{
		register_as(TYPE::PHYSICAL);
		update_aabb();
	}
	virtual void set_x(float nx) override
	{
		invalidate();
		update_aabb();
		Positional::set_x(nx);
	}
	virtual void set_y(float ny) override
	{
		invalidate();
		update_aabb();
		Positional::set_y(ny);
	}
	virtual void set_size(float nz) 
	{
		invalidate();
		Spacial::set_size(nz);
		update_aabb();
	}
	virtual void set_ratio(float nr)
	{
		invalidate();
		Spacial::set_ratio(nr);
		update_aabb();
	}
	virtual void set_rotation(float nd)
	{
		invalidate();
		Directional::set_rotation(nd);
		update_aabb();
	}
	bool probeaabb(float tx, float ty) const
	{
		return bounding_box.probe(tx, ty);
	}
	bool probeaabb(const Physical& other) const
	{
		return bounding_box.collides(other.bounding_box);
	}
	bool check_point(float tx, float ty)
	{
		if (probeaabb(tx, ty))
		{
			update_hitbox();
			return hitbox[active_hb].test_point(tx, ty);
		}
		return false;
	}
	virtual vec2d calculate_collision(Physical& other)
	{
		update_hitbox();
		other.update_hitbox();
		validate();
		vec2d hit_offset = hitbox[active_hb].hits(other.hitbox[other.active_hb]);
		return hit_offset;
	}
	void disable_collision() { active = false; }
	void enable_collision() { active = true; }
protected:
	Physical& operator=(const Physical& other);

	virtual bool solid(const vec2d extra_data = { 0.0, 0.0 }) { return false; }
	virtual bool is_static() const { return false; }

	virtual void resolve_collision(vec2d collision, Physical& other) { return; }
	virtual void on_aabb_collision(Physical& other) { return; }
	virtual void on_physics_test() { return; }
	virtual void on_probe(Object* optional = nullptr) { return; }
	virtual void after_physics_test() { return; }
	void invalidate()
	{
		up_to_date = false;
	}
	void validate()
	{
		up_to_date = true;
	}
	void update_hitbox()
	{
		if (up_to_date) return;
		hitbox[active_hb].update(get_x(), get_y(), get_rotation(), get_size(), get_ratio());
	}
	void update_aabb()
	{
		bounding_box.update(get_x(), get_y(), get_rotation(), get_size());
	}
private:
	friend class Physics;
	Physics* phys_handler = nullptr;
	bool up_to_date, active = true;
	AABB bounding_box;
	int active_hb;
	std::vector<Hitbox> hitbox;
};
