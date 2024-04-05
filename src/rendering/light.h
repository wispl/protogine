#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace proto {

struct GlobalLight {
	GlobalLight() = default;
	GlobalLight(glm::vec3 direction, glm::vec3 color)
		: direction(direction), color(color) {};
	glm::vec3 direction{0.0f};
	glm::vec3 color{1.0f};
};

struct PointLight {
	PointLight() = default;
	PointLight(glm::vec3 position, glm::vec3 color)
		: position(glm::vec4(position, 1.0f)), color(glm::vec4(color, 1.0f))
	{};
	glm::vec3 get_position() const { return glm::vec3(position); };
	void set_position(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;
	};
	void set_color(int r, int g, int b) {
		color.x = r / 255;
		color.y = g / 255;
		color.z = b / 255;
	};
	void set_attentuation(float c, float l, float q) {
		constant = c;
		linear = l;
		quadratic = q;
	};
private:
	glm::vec4 position{0.0f, 0.0f, 0.0f, 1.0f};
	glm::vec4 color{1.0f};
	float constant{1.0f};
	float linear{0.09f};
	float quadratic{0.032f};

	float padding;
};

} // namespace proto
