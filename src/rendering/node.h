#pragma once

#include "../extern/gl.h"

#include "../graphics/mesh.h"

#include <glm/glm.hpp>

#include <memory>

namespace proto {

struct Node {
	std::shared_ptr<Mesh> mesh;
	glm::mat4 model;

	void draw(GLuint vao) const {
		mesh->draw(vao);
	}
};
	
}
