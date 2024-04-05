#include "input.h"

#include "../services/log.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace proto::input {

static void process_button(uint32_t button, bool pressed, bool held);

static MappedData mapped_data;
static std::vector<InputCallback> callbacks;
static std::vector<ActionSet> actionsets;
static ActionSet* enabled;

Action action(const char* name, uint32_t code, uint32_t mapping)
{
	return { name, code, mapping, false };
}

Action state(const char* name, uint32_t code, uint32_t mapping)
{
	return { name, code, mapping, true };
}

bool MappedData::has_action(uint32_t action) const
{
	return actions.find(action) != actions.end();
}

bool MappedData::has_state(uint32_t state) const
{
	return states.find(state) != states.end();
}

void initialize(std::vector<ActionSet> as)
{
	if (actionsets.size() > 0) {
		proto::log::warn("[Input] input has already been initialized!");
		return;
	}
	actionsets = as;
}

void process(const SDL_Event &e)
{
	// TODO: Should only warn once instead of every frame
	if (enabled == nullptr) {
		proto::log::warn("[Input] no actionset enabled!");
		return;
	}

	switch (e.type) {
	case SDL_EVENT_KEY_DOWN: process_button(e.key.keysym.sym, true, e.key.repeat); break;
	case SDL_EVENT_KEY_UP:   process_button(e.key.keysym.sym, false, true); break;
	default: break;
	}
}

static void process_button(uint32_t button, bool pressed, bool held)
{
	auto action = enabled->actions.find(button);
	auto state = enabled->states.find(button);
	if (pressed) {
		if (!held && action != enabled->actions.end()) {
			mapped_data.actions.insert(action->second);
		}
		if (state != enabled->states.end()) {
			mapped_data.states.insert(state->second);
		}
	} else {
		if (action != enabled->actions.end()) {
			mapped_data.actions.erase(action->second);
		}
		if (state != enabled->actions.end()) {
			mapped_data.states.erase(state->second);
		}
	}
}

void update(float dt)
{
	mapped_data.delta_time = dt;
	for (const auto& callback : callbacks) {
		callback(mapped_data);
	}
	// clear for next frame
	mapped_data.actions.clear();
}

void enable_actionset(uint32_t id)
{
	if (actionsets.size() == 0) {
		proto::log::warn("[Input] no action sets found, did you call proto::input::initialize?", id);
		return;
	}

	for (auto& actionset : actionsets) {
		if (actionset.id == id) {
			enabled = &actionset;
			return;
		}
	}
	proto::log::warn("[Input] could not enable actionset of id %s!", id);
}

void add_callback(InputCallback callback)
{
	callbacks.push_back(callback);
}

} // namespace proto::input
