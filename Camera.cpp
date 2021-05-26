#include "Camera.h"
#include "Debug.h"
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>

Camera::Camera(float width, float height)
{
	view = glm::mat4(1.0f);
	projection = glm::ortho(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f);
	GLC(glGenBuffers(1, &ubo));
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
	GLC(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW));
	GLC(glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo));
	GLC(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection));
	GLC(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view));
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void Camera::clear()
{
	GLC(glDeleteBuffers(1, &ubo));
}

void Camera::use()
{
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
	GLC(glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo));
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void Camera::set_projection(float width, float height)
{
	projection = glm::ortho(-(width / 2), (width / 2), -(height / 2), (height / 2), 0.0f, 1.0f);
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
	GLC(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection));
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void Camera::set_x(float nx)
{
	x = nx;
	view = glm::translate(glm::mat4(1.0f), glm::vec3(nx, 0.0f, 0.0f));
	update_view();
}

void Camera::set_y(float ny)
{
	y = ny;
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, ny, 0.0f));
	update_view();
}

void Camera::move(float nx, float ny)
{
	x = nx;
	y = ny;
	view = glm::translate(view, glm::vec3(nx, ny, 0.0f));
	update_view();
}

void Camera::update_view()
{
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
	GLC(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view));
	GLC(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
