#include "application.h"
#include "inputcodes.h"
#include "platform/sdl.h"

#include "services/global_scene.h"
#include "services/log.h"
#include "services/resource_manager.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char *argv[])
{
	auto system = sdl::make_sdlsystem(SDL_INIT_VIDEO);
	if (!system) {
		sdl::panic("Failed to create SDL system");
		return -1;
	}

	uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	auto window = sdl::make_window("Prototype", 1280, 800, flags);
	if (!window) {
		sdl::panic("Failed to create window");
		return -1;
	}

	bool quit = false;
	auto app = proto::Application(window.get());
	app.initialize();

	proto::input::ActionSet main(
		to_underlying(ActionSet::MAIN),
		std::vector{
			proto::input::state("Move Foward",   to_underlying(State::MOVE_FORWARD),  SDLK_W),
			proto::input::state("Move Left",     to_underlying(State::MOVE_LEFT),     SDLK_A),
			proto::input::state("Move Backward", to_underlying(State::MOVE_BACKWARD), SDLK_S),
			proto::input::state("Move Right",    to_underlying(State::MOVE_RIGHT),    SDLK_D),
			proto::input::action("Quit", 	     to_underlying(Action::QUIT),         SDLK_Q)
		}
	);
	proto::input::initialize(std::vector{ main });
	proto::input::enable_actionset(to_underlying(ActionSet::MAIN));
	proto::input::add_callback([&quit](auto data) {
		if (data.has_action(to_underlying(Action::QUIT))) {
			quit = true;
		}
	});
	proto::input::add_callback([](auto data) {
		auto camera = proto::scene::get_scene().camera;
		if (data.has_state(to_underlying(State::MOVE_FORWARD))) {
			camera->move(proto::Direction::FORWARD, data.delta_time);
		}
		if (data.has_state(to_underlying(State::MOVE_BACKWARD))) {
			camera->move(proto::Direction::BACKWARD, data.delta_time);
		}
		if (data.has_state(to_underlying(State::MOVE_LEFT))) {
			camera->move(proto::Direction::LEFT, data.delta_time);
		}
		if (data.has_state(to_underlying(State::MOVE_RIGHT))) {
			camera->move(proto::Direction::RIGHT, data.delta_time);
		}
	});

	auto cube = std::make_shared<proto::Mesh>(proto::Mesh());
	auto mesh = proto::resources::load_mesh("assets/backpack/backpack.obj");

	proto::Scene& scene = proto::scene::get_scene();
	scene.global_light = proto::GlobalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f));
	scene.lights[0] = proto::PointLight(glm::vec3( 0.7f,  0.2f,  2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	scene.lights[1] = proto::PointLight(glm::vec3( 2.3f, -3.3f,  2.0f), glm::vec3(0.3f, 0.1f, 0.1f));
	scene.lights[2] = proto::PointLight(glm::vec3(-4.0f,  2.0f, -12.0f), glm::vec3(0.4f, 0.1f, 0.1f));
	scene.lights[3] = proto::PointLight(glm::vec3( 3.0f,  1.0f,  2.0f), glm::vec3(0.3f, 0.1f, 0.1f));
	glm::mat4 model(1.0f);
	scene.add_node(proto::Node{ mesh, model });

	model = glm::translate(model, glm::vec3(1.0f));
	scene.add_node(proto::Node{ cube, model });

	while (!quit) {
		app.run();
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED: quit = true; break;
			case SDL_EVENT_WINDOW_RESIZED: 	       app.resize(); break;
			case SDL_EVENT_MOUSE_MOTION: {
				auto camera = proto::scene::get_scene().camera;
				camera->turn(e.motion.xrel, -e.motion.yrel);
				break;
				}
			}
			proto::input::process(e);
		}
	}
	return 0;
}
