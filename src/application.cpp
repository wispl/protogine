#include "application.h"

#include "platform/window.h"
#include "platform/graphics.h"

#include "services/global_scene.h"
#include "services/input.h"
#include "services/log.h"

#include <SDL3/SDL.h>

#include <memory>

namespace proto {

void Application::initialize()
{
	initialized = true;
	graphics.set_vsync(true);
	graphics.set_relative_mouse(true);
}

void Application::run()
{
	float current_frame = SDL_GetTicks();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;

	update();
	render();
}

void Application::update()
{
	proto::input::update(delta_time);
}

void Application::render()
{
	renderer.render(proto::scene::get_scene());
	window.swapbuffer();
}

void Application::resize()
{
	renderer.resize(window.get_width(), window.get_height());
}

} // namespace proto

