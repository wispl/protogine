#include "resource_manager.h"

#include "log.h"

#include "../graphics/mesh.h"
#include "../graphics/texture.h"

#include "../extern/stb_image.h"

#include <memory>
#include <unordered_map>
#include <stdexcept>

namespace proto {

namespace resources {

static std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

std::shared_ptr<Mesh> load_mesh(const std::string& path)
{
	if (auto result = meshes.find(path); result != meshes.end()) {
		return result->second;
	}

	auto mesh = std::make_shared<Mesh>(path);
	meshes.emplace(path, mesh);
	return mesh;
}

std::shared_ptr<Texture> load_texture(const std::string& path)
{
	if (auto result = textures.find(path); result != textures.end()) {
		return result->second;
	}

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data) {
		throw std::runtime_error("Unable to load file!");
		stbi_image_free(data);
	}

	TextureConfig config;
	config.type = TextureType::TEXTURE_2D;
	config.width = width;
	config.height = height;
	config.generate_mipmaps = true;

	switch (nrChannels) {
	case 1: config.format = TextureFormat::RED; break;
	case 3: config.format = TextureFormat::RGB; break;
	case 4: config.format = TextureFormat::RGBA; break;
	}

	auto texture = std::make_shared<Texture>(data, config);
	textures.emplace(path, texture);

	stbi_image_free(data);

	return texture;
}

} // namespace resources

} // namespace proto
