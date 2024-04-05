#pragma once

#include <glm/mat4x4.hpp>

struct RenderObject {
	int count;
	int index;
	Pipeline* pipeline;
	glm::mat4 transform;
};

class Renderable {
	virtual void draw(const glm::mat4& mat, DrawContext& ctx) = 0;
};
