#pragma once
#include "Texture.h"
#include "Spacial.h"
#include "Directional.h"
#include "Batch.h"
#include "Positional.h"
#include "Object.h"

#include <vector>
#include <cstdio>
#include <initializer_list>


class DynamicLayer;
// Vector Safe
class Drawable : virtual public Directional, virtual public Spacial, virtual public Positional
{
public:
	Drawable(float x = 0.0, float y = 0.0, float size = 1.0, float deg = 0.0, const std::vector<const char*>& init = {}) 
		: Positional(x, y), Spacial(size), Directional(deg) 
	{
		register_as(TYPE::DRAWABLE);
		for (auto fname : init)
		{
			sprites.emplace_back(Texture::pool(fname));
		}
	}
	Drawable(std::initializer_list<const char*> init)
	{
		register_as(TYPE::DRAWABLE);
		for (auto fname : init)
		{
			sprites.emplace_back(Texture::pool(fname));
		}
	}
	Drawable(const std::vector<const char*>& init)
	{
		register_as(TYPE::DRAWABLE);
		for (auto fname : init)
		{
			sprites.emplace_back(Texture::pool(fname));
		}
	}
	operator vertex() const
	{
		return { get_x(), get_y(), get_rotation(), opacity, get_size(), sprites.at(active).get_ratio(), float(idx) };
	}
	operator Texture() const
	{
		return active_texture();
	}
	Texture active_texture() const
	{
		return sprites.at(active);
	}
	int active_texture_idx() const { return active; }
	virtual void change_active_texture(int nidx)
	{
		active = nidx;
		update_batch();
	}
	virtual void set_x(float nx) override 
	{ 
		Positional::set_x(nx); 
		update_batch();
	}
	virtual void set_y(float nx) override
	{
		Positional::set_y(nx);
		update_batch();
	}
	virtual void set_rotation(float nd) override
	{
		Directional::set_rotation(nd);
		update_batch();
	}
	virtual void set_size(float nz) override
	{
		Spacial::set_size(nz);
		update_batch();
	}
	virtual void set_opacity(float op)
	{
		opacity = op;
	}
	float get_opacity() const { return opacity; }
protected:
	Drawable& operator=(const Drawable& other);
	Drawable& operator=(Drawable&& other) noexcept;
	void update_batch()
	{
		if (batch)
		{
			batch->set(idx, *this, active_texture());
		}
	}
private:
	friend class DynamicLayer;
	friend class Engine;
	void unset()
	{
		if (batch)
		{
			batch->remove(idx);
			batch = nullptr;
			layer = nullptr;
		}
	}
	void reg(DynamicLayer* lay, DynamicBatch* btc, int nidx)
	{
		layer = lay;
		batch = btc;
		idx = nidx;
	}
	DynamicBatch* batch_ptr() const
	{
		return batch;
	}
	DynamicLayer* lay_ptr() const
	{
		return layer;
	}
	std::vector<Texture> sprites;
	DynamicBatch* batch = nullptr;
	DynamicLayer* layer = nullptr;
	int idx = -1, active = 0;
	float opacity = 1.0;
};
