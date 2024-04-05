#pragma once

#include "camera.h"
#include "light.h"
#include "scene.h"

#include "../graphics/mesh.h"
#include "../graphics/pipeline.h"
#include "../graphics/texture.h"

#include "../extern/gl.h"
#include "../platform/window.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <memory>
#include <string>

namespace proto {

class Renderer {
public:
	Renderer();
	void render(const Scene& scene);
	void resize(int w, int h);
private:
	GLuint vao;
	GLuint lights_ubo;
	Pipeline default_pipeline;

	int width, height;
	bool is_resized = false;
};

} // namespace proto
