#include "texture.h"

#include "../extern/gl.h"

namespace proto {

static GLenum convert_format(TextureFormat format);
static GLenum convert_format_type(TextureFormat format);
static GLenum convert_type(TextureType type);
static GLenum convert_wrap(TextureWrap wrap);
static GLenum convert_filter(TextureFilter filter);

Texture::Texture(const void* data, const TextureConfig& config)
{
	glCreateTextures(convert_type(config.type), 1, &id);
	if (config.type	== TextureType::TEXTURE_2D) {
		glTextureStorage2D(id, 1, convert_format_type(config.format), config.width, config.height);
		glTextureSubImage2D(id, 0, 0, 0, config.width, config.height, convert_format(config.format), GL_UNSIGNED_BYTE, data);
	}

	if (config.generate_mipmaps) {
		glGenerateTextureMipmap(id);
	}
}

void Texture::bind(int i) const
{
	glBindTextureUnit(i, id);
}

void Texture::set_wrap(TextureWrap wrap) const
{
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, convert_wrap(wrap));
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, convert_wrap(wrap));
}

void Texture::set_minmag_filter(TextureFilter near, TextureFilter far) const
{
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, convert_filter(near));
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, convert_filter(far));
}

/// Conversion from custom types to OpenGL types

static GLenum convert_format(TextureFormat format) {
	switch (format) {
	case TextureFormat::RED:  return GL_RED;
	case TextureFormat::RG:   return GL_RG;
	case TextureFormat::RGB:  return GL_RGB;
	case TextureFormat::RGBA: return GL_RGBA;
	}
	return GL_NONE;
}

static GLenum convert_format_type(TextureFormat format) {
	switch (format) {
	case TextureFormat::RED:  return GL_R8;
	case TextureFormat::RG:   return GL_RG8;
	case TextureFormat::RGB:  return GL_RGB8;
	case TextureFormat::RGBA: return GL_RGBA8;
	}
	return GL_NONE;
}

static GLenum convert_type(TextureType type)
{
	switch (type) {
	case TextureType::TEXTURE_2D: return GL_TEXTURE_2D;
	}
	return GL_NONE;
}

static GLenum convert_wrap(TextureWrap wrap)
{
	switch (wrap) {
	case TextureWrap::REPEAT:		return GL_REPEAT;
	case TextureWrap::NEAREST:		return GL_NEAREST;
	case TextureWrap::LINEAR:		return GL_LINEAR;
	case TextureWrap::MIRRORED_REPEAT:	return GL_MIRRORED_REPEAT;
	case TextureWrap::CLAMP_TO_EDGE:	return GL_CLAMP_TO_EDGE;
	case TextureWrap::CLAMP_TO_BORDER:	return GL_CLAMP_TO_BORDER;
	case TextureWrap::MIRROR_CLAMP_TO_EDGE: return GL_MIRROR_CLAMP_TO_EDGE;
	}
	return GL_NONE;
}

static GLenum convert_filter(TextureFilter filter)
{
	switch (filter) {
	case TextureFilter::NEAREST: 		    return GL_NEAREST;
	case TextureFilter::LINEAR:  		    return GL_LINEAR;
	case TextureFilter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
	case TextureFilter::LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
	case TextureFilter::NEAREST_MIPMAP_LINEAR:  return GL_NEAREST_MIPMAP_LINEAR;
	case TextureFilter::LINEAR_MIPMAP_LINEAR:   return GL_LINEAR_MIPMAP_LINEAR;
	}
	return GL_NONE;
}

} // end namespace proto
