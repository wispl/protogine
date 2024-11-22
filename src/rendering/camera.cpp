#include "camera.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace proto {

Camera::Camera()
{
	update_vectors();
}

glm::mat4 Camera::get_view() const
{
	return glm::lookAt(position, position + front, WORLD_UP);
}

glm::vec3 Camera::get_position() const
{
	return position;
}

void Camera::turn(float x_offset, float y_offset)
{
	yaw += (x_offset * sensitivity);
        pitch += (y_offset * sensitivity);
	pitch = std::clamp(pitch, -89.0f, 89.0f);
        update_vectors();
}

void Camera::move(Direction direction, float dt)
{
	float velocity = dt * speed;
	switch(direction) {
	case Direction::FORWARD:  position += front * velocity; break;
	case Direction::BACKWARD: position -= front * velocity; break;
	case Direction::LEFT: 	  position -= right * velocity; break;
	case Direction::RIGHT: 	  position += right * velocity; break;
	}
}

void Camera::move_to(glm::vec3 vec)
{
	position = vec;
}

void Camera::set_speed(float s)
{
	speed = s;
}

void Camera::set_sensitivity(float s)
{
	sensitivity = s;
}

void Camera::update_vectors()
{
	front = glm::normalize(glm::vec3(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	));
	right = glm::normalize(glm::cross(front, WORLD_UP));
	up = glm::normalize(glm::cross(right, front));
}

} // namespace proto
