#pragma once

#include <SDL3/SDL.h>

namespace proto {

using window_type = SDL_Window*;

class Window {
public:
	Window(window_type window) : window(window) {}

	void swapbuffer() const
	{
		SDL_GL_SwapWindow(window);
	}

	void close() const {
		SDL_Event quit_event;
		quit_event.type = SDL_EVENT_QUIT;
		SDL_PushEvent(&quit_event);
	}

	int get_width() const
	{
		int width;
		SDL_GetWindowSize(window, &width, NULL);
		return width;
	}

	int get_height() const
	{
		int height;
		SDL_GetWindowSize(window, NULL, &height);
		return height;
	}

	const char* get_title() const
	{
		return SDL_GetWindowTitle(window);
	}
private:
	window_type window;
};

} // namespace proto
