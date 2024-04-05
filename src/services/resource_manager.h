#pragma once

#include <string>
#include <memory>

namespace proto {
	class Mesh;
	class Texture;

	namespace resources {
		std::shared_ptr<Mesh> load_mesh(const std::string& path);
		std::shared_ptr<Texture> load_texture(const std::string& path);
	}
}
