#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

enum class Direction {
	FORWARD,
	LEFT,
	RIGHT,
	BACKWARD
};

class Camera {
public:
	Camera();
	glm::mat4 get_view() const;
	glm::vec3 get_position() const;
	void turn(float x_offset, float y_offset);
	void move(Direction direction, float dt);
	void move_to(glm::vec3 vec);
	void set_speed(float s);
	void set_sensitivity(float s);
private:
	static constexpr glm::vec3 WORLD_UP{0.0f, 1.0f, 0.0f};
	glm::vec3 position{0.0f};
	glm::vec3 front{0.0f};
	glm::vec3 right{0.0f};
	glm::vec3 up{0.0f};
	float yaw{-90.0f};
	float pitch{0.0f};
	float speed = 0.01f;
	float sensitivity = 0.25f;

	void update_vectors();
};
