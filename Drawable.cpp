#include "Drawable.h"
#include "Layer.h"
#include "Debug.h"


/*
Drawable::Drawable(const Drawable& other)
	: Directional(other), Spacial(other), Positional(other),
	idx(other.idx), sprites(other.sprites), opacity(other.opacity),
	active(other.active), batch(nullptr), layer(nullptr)
{
	register_as(TYPE::DRAWABLE);
	DBG(printf("Copie Constructed Drawable: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
}

Drawable::Drawable(Drawable&& other) noexcept
	: Directional(other), Spacial(other), Positional(other),
	active(other.active), idx(other.idx), opacity(other.opacity), sprites(std::move(other.sprites))
{
	register_as(TYPE::DRAWABLE);
	if (other.layer)
	{
		layer = other.layer;
		layer->remove(other);
		layer->add(*this);
	}
	else
	{
		layer = nullptr;
		batch = nullptr;
	}
	DBG(printf("Move Constructed Drawable: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
}

Drawable& Drawable::operator=(const Drawable& other)
{
	if (this != &other)
	{
		opacity = other.opacity;
		active = other.active;
		sprites = other.sprites;
		// layer batch and idx stay untouched only the appearance gets copyed
		update_batch();
		DBG(printf("Copy Assigned Drawable: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}

Drawable& Drawable::operator=(Drawable&& other) noexcept
{
	if (this != &other)
	{
		if (other.layer)
		{
			other.layer->remove(other);
		}
		opacity = other.opacity;
		active = other.active;
		sprites = std::move(other.sprites);
		// yet again layer batch and idx stay untouched only the appearance gets copyed
		update_batch();
		DBG(printf("Move Assigned Drawable: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}
*/

Drawable& Drawable::operator=(const Drawable& other)
{
	if (this != &other)
	{
		opacity = other.opacity;
		active = other.active;
		sprites = other.sprites;
		// layer batch and idx stay untouched only the appearance gets copyed
		update_batch();
		DBG(printf("Copy Assigned Drawable: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}

Drawable& Drawable::operator=(Drawable&& other) noexcept
{
	if (this != &other)
	{
		if (other.layer){
			other.layer->remove(other);
		}
		opacity = other.opacity;
		active = other.active;
		sprites = std::move(other.sprites);
		// yet again layer batch and idx stay untouched only the appearance gets copyed
		update_batch();
		DBG(printf("Move Assigned Drawable: %s addr %p got assigned values of %p\n", typeid(*this).name(), this, &other);)
	}
	return *this;
}
