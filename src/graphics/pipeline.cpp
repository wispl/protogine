#include "pipeline.h"

#include "../platform/window.h"
#include "../platform/filesystem.h"
#include "../services/log.h"

#include <glm/mat4x4.hpp>

#include <vector>

namespace proto {

static void check_progam(GLuint progam);
static void check_shader(GLuint shader, const char* path);

Pipeline::Pipeline(const std::vector<Shader>& shaders)
{
	std::vector<GLuint> binaries;

	program = glCreateProgram();
	for (auto& shader : shaders) {
		auto source = filesystem::read_file(shader.path);
		auto str = source.c_str();
		auto binary = glCreateShader(shader.type);
		glShaderSource(binary, 1, &str, nullptr);
		glCompileShader(binary);
		check_shader(binary, shader.path);

		binaries.push_back(binary);
	}
	for (auto& binary : binaries) {
		glAttachShader(program, binary);
	}

	glLinkProgram(program);
	check_progam(program);

	for (auto& binary : binaries) {
		glDeleteShader(binary);
	}
}

void Pipeline::bind()
{
	glUseProgram(program);
}

void Pipeline::unbind()
{
	glUseProgram(0);
}

void Pipeline::set_uint(const char* uniform, unsigned int i) const
{
	glUniform1ui(uniform_location(uniform), i);
}

void Pipeline::set_int(const char* uniform, int i) const
{
	glUniform1i(uniform_location(uniform), i);
}

void Pipeline::set_float(const char* uniform, float f) const
{
	glUniform1f(uniform_location(uniform), f);
}

void Pipeline::set_vec3(const char* uniform, const glm::vec3& vec) const
{
	glUniform3fv(uniform_location(uniform), 1, &vec[0]);
}

void Pipeline::set_mat4(const char* uniform, const glm::mat4& mat) const
{
	glUniformMatrix4fv(uniform_location(uniform), 1, GL_FALSE, &mat[0][0]);
}

GLint Pipeline::uniform_location(const char* uniform) const
{
	return glGetUniformLocation(program, uniform);
}

static void check_progam(GLuint program)
{
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (!result) {
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			std::vector<char> log(length);
			glGetProgramInfoLog(program, length, &length, &log[0]);
			proto::log::error("[OpenGL] Failed to compile pipeline %s:", log.data());
		}
	}
}

static void check_shader(GLuint shader, const char* path)
{
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			std::vector<char> log(length);
			glGetShaderInfoLog(shader, length, &length, &log[0]);
			proto::log::error("[OpenGL] Failed to compile shader %s: %s", path, log.data());
		}
	}
}

} // namespace proto
