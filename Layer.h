#pragma once
#include "Batch.h"
#include "Drawable.h"
#include "Shader.h"

#include <deque>
#include <stack>

class DynamicLayer
{
public:
	DynamicLayer();
	virtual void remove(Drawable& drawable)
	{
		if (drawable.lay_ptr() == this)
		{
			capacity.push(drawable.batch_ptr());
			drawable.unset();
		}
	}
	virtual void add(Drawable& drawable)
	{
		if (capacity.empty())
		{
			if (batches.back().full())
			{
				batches.emplace_back();
			}
			drawable.reg(this, &batches.back(), batches.back().amount());
			batches.back().add(drawable, drawable);
		}
		else
		{
			capacity.top()->add(drawable, drawable);
			drawable.reg(this, capacity.top(), capacity.top()->amount());
			if (capacity.top()->full())
			{
				capacity.pop();
			}
		}
	}
	virtual void draw(/*framebuffer*/);
	void hide() { hidden = true; } void show() { hidden = false; } bool is_hidden() const { return hidden; }
private:
	bool hidden = false;
	Shader shdr;
	std::deque<DynamicBatch> batches;
	std::stack<DynamicBatch*> capacity;
};

