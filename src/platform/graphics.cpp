#include "graphics.h"

#include "window.h"

#include "../extern/gl.h"
#include "../services/log.h"
#include "../platform/sdl.h"

#include <SDL3/SDL.h>

namespace proto {

Graphics::Graphics(window_type window)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	context = SDL_GL_CreateContext(window);
	if (!context) {
		sdl::panic("Failed to create a OpenGL context");
	}

	if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
		sdl::panic("Failed to load OpenGL via glad");
	}
	proto::log::info("Loaded OpenGL version %s\n", glGetString(GL_VERSION));
}

Graphics::~Graphics()
{
	SDL_GL_DestroyContext(context);
}

void Graphics::set_window(window_type window)
{
	SDL_GL_MakeCurrent(window, context);
}

void Graphics::set_vsync(bool vsync) const
{
	SDL_GL_SetSwapInterval(vsync);
}

void Graphics::set_relative_mouse(window_type window, bool relative_mouse) const
{
	SDL_SetWindowRelativeMouseMode(window, relative_mouse);
}

} // namespace proto
