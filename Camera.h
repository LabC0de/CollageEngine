#pragma once
#include "Positional.h"
#include "Collected.h"
#include <glm.hpp>


class Camera : public Positional, public Collected
{
public:
	Camera(float width, float height);
	~Camera()
	{
		dtor();
	}
	virtual void clear() override;
	void use();
	virtual void set_projection(float width, float height);
	virtual void set_x(float nx) override;
	virtual void set_y(float ny) override;
	virtual void move(float nx, float ny) override;
private:
	void update_view();
	glm::mat4 projection, view;
	unsigned ubo;
};