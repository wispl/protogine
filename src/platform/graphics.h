#pragma once

#include "window.h"

#include <SDL3/SDL.h>

namespace proto {

class Graphics {
public:
	Graphics(window_type window);
	~Graphics();
	void set_window(window_type window);
	void set_vsync(bool vsync) const;
	void set_relative_mouse(bool relative_mouse) const;
private:
	SDL_GLContext context;
};

} // namespace proto
