#pragma once

#include "../extern/gl.h"

#include "../utils/log.hpp"
#include "../utils/sdl.h"

#include <SDL3/SDL.h>

class Graphics {
public:
	Graphics(window_type window)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		context = sdl::make_context(window);
		if (!context) {
			sdl::panic("Failed to create a OpenGL context");
		}

		if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
			sdl::panic("Failed to load OpenGL via glad");
		}
		proto::log::info("Loaded OpenGL version %s\n", glGetString(GL_VERSION));
	}

	void set_window(window_type window)
	{
		SDL_GL_MakeCurrent(window, context);
	}

	void set_vsync(bool vsync) const
	{
		SDL_GL_SetSwapInterval(vsync);
	}

	void set_relative_mouse(bool relative_mouse) const
	{
		SDL_SetRelativeMouseMode(relative_mouse);
	}
private:
	context_ptr context;
};

