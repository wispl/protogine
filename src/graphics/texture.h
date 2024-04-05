#pragma once

#include "../extern/gl.h"

namespace proto {

enum class TextureFormat {
	RED,
	RG,
	RGB,
	RGBA
};

enum class TextureType {
	TEXTURE_2D
};

enum class TextureWrap {
	REPEAT,
        NEAREST,
        LINEAR,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRROR_CLAMP_TO_EDGE
};

enum class TextureFilter {
	NEAREST,
	NEAREST_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
        LINEAR,
	LINEAR_MIPMAP_NEAREST,
	LINEAR_MIPMAP_LINEAR
};

struct TextureConfig {
	TextureType type;
	TextureFormat format;
	int width;
        int height;
	bool generate_mipmaps;
};

class Texture {
public:
	Texture(const void* data, const TextureConfig& config);
	void bind(int i) const;
	void set_wrap(TextureWrap wrap) const;
	void set_minmag_filter(TextureFilter near, TextureFilter far) const;
private:
	GLuint id;
};

} // namespace proto
