#include "mesh.h"

#include "texture.h"

#include "../services/resource_manager.h"
#include "../services/log.h"

#include "../extern/gl.h"
#include "../extern/tiny_obj_loader.h"
#include "../platform/filesystem.h"

#include <stdexcept>

namespace proto {

Mesh::Mesh(const std::string& path)
{
	auto asset_dir = filesystem::parent_dir(path.c_str());
	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = asset_dir;

	if (!reader.ParseFromFile(path, reader_config)) {
		if (!reader.Error().empty()) {
			proto::log::error("[TinyObjReader] %s", reader.Error().c_str());
		}
		throw std::runtime_error("Unable to load mesh!");
	}

	if (!reader.Warning().empty()) {
		proto::log::warn("[TinyObjReader] %s", reader.Warning().c_str());
	}

	const auto& attrib = reader.GetAttrib();
	const auto& materials = reader.GetMaterials();
	const auto& shapes = reader.GetShapes();

	size_t index_offset = 0;
	for (const auto& shape : shapes) {
		for (const auto& index: shape.mesh.indices) {
			vertices.push_back(Vertex(
				// vertices
				attrib.vertices[3*index.vertex_index+0],
				attrib.vertices[3*index.vertex_index+1],
				attrib.vertices[3*index.vertex_index+2],
				// normal
				attrib.normals[3*index.normal_index+0],
				attrib.normals[3*index.normal_index+1],
				attrib.normals[3*index.normal_index+2],
				// uv
				attrib.texcoords[2*index.texcoord_index+0],
				1.0f - attrib.texcoords[2*index.texcoord_index+1]
			));
			// TODO: deduplicate vertices
			indices.push_back(indices.size());
		}
		auto material = materials[shape.mesh.material_ids[0]];

		SubMesh submesh(
			index_offset,
			shape.mesh.indices.size(),
			Material(
				proto::resources::load_texture(asset_dir + "/" + material.diffuse_texname),
				proto::resources::load_texture(asset_dir + "/" + material.specular_texname)
			)
		);
		index_offset += shape.mesh.indices.size();

		submeshes.push_back(submesh);
	}

	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	glNamedBufferStorage(vbo, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(ebo, sizeof(int) * indices.size(), &indices[0], GL_DYNAMIC_STORAGE_BIT);
}

Mesh::Mesh()
{
	vertices = {
		// Back
		Vertex(-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f),
		Vertex( 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f),
		Vertex( 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f),
		Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f),
		// Front
		Vertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f),
		Vertex( 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f),
		Vertex( 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f),
		Vertex(-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f),
		// Left
		Vertex(-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f),
		Vertex(-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f),
		Vertex(-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f),
		// Right
		Vertex( 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f),
		Vertex( 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f),
		Vertex( 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f),
		Vertex( 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f),
		// Bottom
		Vertex(-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f),
		Vertex( 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f),
		Vertex( 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f),
		Vertex(-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f),
		// Top
		Vertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f),
		Vertex( 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f),
		Vertex( 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f),
		Vertex(-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f),
	};

	indices = {
		0, 1, 2, 2, 3, 0, 	// Back
		4, 5, 6, 6, 7, 4, 	// Front
		8, 9, 10, 10, 11, 8, 	// Left
		12, 13, 14, 14, 15, 12, // Right
		16, 17, 18, 18, 19, 16, // Bottom
		20, 21, 22, 22, 23, 20, // Top
	};
	auto diffuse = proto::resources::load_texture("assets/container.png");
	auto specular = proto::resources::load_texture("assets/container_specular.png");
	submeshes.push_back(SubMesh(0, indices.size(), Material(diffuse, specular)));

	glCreateBuffers(1, &vbo);
	glCreateBuffers(1, &ebo);

	glNamedBufferStorage(vbo, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(ebo, sizeof(int) * indices.size(), &indices[0], GL_DYNAMIC_STORAGE_BIT);
}

void Mesh::draw(GLuint vao) const
{
	glBindVertexArray(vao);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(vao, ebo);

	for (const auto& submesh : submeshes) {
		submesh.draw();
	}
}

void SubMesh::draw() const
{
	material.bind();
	glDrawElementsBaseVertex(GL_TRIANGLES, index_size, GL_UNSIGNED_INT, nullptr, index_start);
}

void Material::bind() const
{
	diffuse->bind(0);
	specular->bind(1);
}

} // namespace proto
