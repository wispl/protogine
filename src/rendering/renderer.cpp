#include "renderer.h"

#include "../services/resource_manager.h"
#include "../services/log.h"

#include "../graphics/mesh.h"
#include "../graphics/pipeline.h"
#include "../graphics/texture.h"

#include "../extern/gl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL3/SDL.h>

#include <string>
#include <sstream>

namespace proto {

static void debug_callback(GLenum source, GLenum type, GLuint id,
			   GLenum severity, GLsizei length,
			   const GLchar* message, const void* userParam);

Renderer::Renderer()
{
	// Enable debug callbacks
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debug_callback, nullptr);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

	auto shaders = std::vector {
		Shader{"shaders/main.vert", GL_VERTEX_SHADER},
		Shader{"shaders/main.frag", GL_FRAGMENT_SHADER}
	};
	default_pipeline = Pipeline(shaders);
	default_pipeline.bind();

	glCreateVertexArrays(1, &vao);

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);

	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribBinding(vao, 2, 0);
	glBindVertexArray(vao);

	glCreateBuffers(1, &lights_ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_ubo);
	glNamedBufferStorage(lights_ubo, 4 * sizeof(PointLight), NULL, GL_DYNAMIC_STORAGE_BIT);
}

void Renderer::render(const Scene& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	default_pipeline.bind();

	if (is_resized) {
		default_pipeline.set_mat4(
			"projection",
			glm::perspective(glm::radians(45.0f),
		    			 static_cast<float>(width) / height,
					 1.0f,
					 1000.0f)
		);
		glViewport(0, 0, width, height);
		is_resized = false;
	}

	default_pipeline.set_int("material.diffuse", 0);
	default_pipeline.set_int("material.specular", 1);
	default_pipeline.set_float("material.shininess", 32.0f);

	default_pipeline.set_mat4("view", scene.camera->get_view());
	default_pipeline.set_vec3("view_pos", scene.camera->get_position());

	glNamedBufferSubData(lights_ubo, 0, sizeof(scene.lights), &scene.lights[0]);
	default_pipeline.set_int("light_idx", -1);
	default_pipeline.set_vec3("global_light.direction", scene.global_light.direction);
	default_pipeline.set_vec3("global_light.color", scene.global_light.color);

	for (auto node : scene.nodes) {
		default_pipeline.set_mat4("model", node.model);
		node.draw(vao);
	}

	/* for (int i = 0; i < scene.lights.size(); ++i) { */
	/* 	glm::mat4 model(1.0f); */
	/* 	model = glm::translate(model, scene.lights[i].get_position()); */
	/* 	default_pipeline.set_mat4("model", model); */
	/* 	default_pipeline.set_int("light_idx", i); */
	/* } */
}

void Renderer::resize(int w, int h)
{
	width = w;
	height = h;
	is_resized = true;
}

static void debug_callback(GLenum source, GLenum type, GLuint id,
			   GLenum severity, GLsizei length,
			   const GLchar* message, const void* userParam
) {
	auto const src_str = [source]() {
		switch (source) {
		case GL_DEBUG_SOURCE_API:  	      return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER:  	      return "OTHER";
		}
		return "UNKNOWN";
	}();
	auto const type_str = [type]() {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR: 	        return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED";
		case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER:              return "MARKER";
		case GL_DEBUG_TYPE_OTHER:               return "OTHER";
		}
		return "UNKNOWN";
	}();
	auto const level = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return proto::log::Level::DEBUG;
		case GL_DEBUG_SEVERITY_LOW: 	     return proto::log::Level::INFO;
		case GL_DEBUG_SEVERITY_MEDIUM:	     return proto::log::Level::WARN;
		case GL_DEBUG_SEVERITY_HIGH: 	     return proto::log::Level::ERROR;
		}
		return proto::log::Level::DEBUG;
	}();

	proto::log::log(level, "[OpenGL: %s] %s: %s", src_str, type_str, message);
}

} // end namespace proto
