#pragma once

#include "../extern/gl.h"

#include <glm/mat4x4.hpp>

#include <vector>

namespace proto {

struct Shader {
	const char* path;
	GLenum type;
};

class Pipeline {
public:
	Pipeline() = default;
	Pipeline(const std::vector<Shader>& shaders);
	void bind();
	void unbind();
	void set_uint(const char* uniform, unsigned int i) const;
	void set_int(const char* uniform, int i) const;
	void set_float(const char* uniform, float i) const;
	void set_vec3(const char* uniform, const glm::vec3& vec) const;
	void set_mat4(const char* uniform, const glm::mat4& mat) const;
	GLint uniform_location(const char* uniform) const;

private:
	GLuint program;
};

} // namespace proto
