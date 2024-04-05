#pragma once

#include "../extern/gl.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <string>
#include <vector>

namespace proto {

class Texture;

struct Material {
	Material(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular) :	
		diffuse(diffuse), specular(specular)
	{};
	void bind() const;
	std::shared_ptr<Texture> diffuse;
	std::shared_ptr<Texture> specular;
};

struct Vertex {
	Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) :
	        pos(pos), normal(normal), uv(uv) {};

	Vertex(float x, float y, float z,
	       float nx, float ny, float nz,
	       float uv_x, float uv_y)
	      :	
	        pos(glm::vec3(x, y, z)),
		normal(glm::vec3(nx, ny, nz)),
		uv(glm::vec2(uv_x, uv_y)) {};

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct SubMesh {
public:	
	SubMesh(GLuint index_start, GLuint index_size, Material material) :
		index_start(index_start),
		index_size(index_size),
		material(material)
	{};
	void draw() const;
private:
	GLuint index_start, index_size;
	Material material;
};

class Mesh {
public:
	Mesh();
	Mesh(const std::string& path);
	void draw(GLuint vao) const;
private:
	GLuint vbo, ebo;
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	std::vector<SubMesh> submeshes;
};

} // namespace proto

