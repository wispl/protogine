#pragma once

#include "platform/graphics.h"
#include "platform/window.h"
#include "services/input.h"

#include "rendering/renderer.h"

#include <memory>

namespace proto {

class Application {
public:
	Application(window_type window) : window(window), graphics(window) {};
	void initialize();
	void run();
	void resize();
private:
	float delta_time, last_frame = 0;
	bool initialized = false;

	Window window;
	Graphics graphics;
	Renderer renderer;

	void update();
	void render();
};

} // namespace proto
