#include "texture.hpp"

#include "../extern/stb_image.h"
#include "../extern/gl.h"

#include "../utils/log.hpp"

#include <stdexcept>

static GLenum convert_wrap(TextureWrap wrap);
static GLenum convert_filter(TextureFilter filter);

Texture::Texture(const char* path)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data) {
		throw std::runtime_error("Unable to load file!");
	}
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_RGB8, width, height);
	glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateTextureMipmap(texture);

	stbi_image_free(data);
}

void Texture::bind(int i) const
{
	glBindTextureUnit(i, texture);
}

void Texture::set_wrap(TextureWrap wrap) const
{
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, convert_wrap(wrap));
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, convert_wrap(wrap));
}

void Texture::set_minmag_filter(TextureFilter near, TextureFilter far) const
{
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, convert_filter(near));
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, convert_filter(far));
}

/// TODO: throw on non match?
static GLenum convert_wrap(TextureWrap wrap)
{
	switch(wrap) {
		case TextureWrap::REPEAT:       return GL_REPEAT;
		case TextureWrap::MIRROR: 	return GL_MIRRORED_REPEAT;
		case TextureWrap::CLAMP_EDGE:   return GL_CLAMP_TO_EDGE;
		case TextureWrap::CLAMP_BORDER: return GL_CLAMP_TO_BORDER;
	}
	return GL_NONE;
}

static GLenum convert_filter(TextureFilter filter)
{
	switch(filter) {
		case TextureFilter::NEAREST:
			return GL_NEAREST;
                case TextureFilter::LINEAR:
			return GL_LINEAR;
                case TextureFilter::NEAREST_MIPMAP_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;
                case TextureFilter::LINEAR_MIPMAP_NEAREST:
			return GL_LINEAR_MIPMAP_NEAREST;
                case TextureFilter::NEAREST_MIPMAP_LINEAR:
			return GL_NEAREST_MIPMAP_LINEAR;
                case TextureFilter::LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
	}
	return GL_NONE;
}
