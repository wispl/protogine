#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <functional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace proto::input {

struct Action {
	const char* name;
	uint32_t code;
	uint32_t mapping;
	bool is_state;
};

Action action(const char* name, uint32_t code, uint32_t mapping);
Action state(const char* name, uint32_t code, uint32_t mapping);

struct ActionSet {
	uint32_t id;
	std::unordered_map<std::string, Action> stringlookup;
	std::unordered_map<uint32_t, uint32_t> actions;
	std::unordered_map<uint32_t, uint32_t> states;

	ActionSet(uint32_t id, std::vector<Action> actionlist) : id(id)
	{
		for (const auto& action : actionlist) {
			if (action.is_state) {
				states[action.mapping] = action.code;
			} else {
				actions[action.mapping] = action.code;
			}
			stringlookup[action.name] = action;
		}
	}
};

struct MappedData {
	float delta_time;
	std::set<uint32_t> actions;
	std::set<uint32_t> states;

	bool has_action(uint32_t action) const;
	bool has_state(uint32_t state) const;
};

using InputCallback = std::function<void(MappedData)>;

void initialize(std::vector<ActionSet> as);
void enable_actionset(uint32_t id);
void add_callback(InputCallback callback);

void process(const SDL_Event& e);
void update(float dt);

} // namespace proto::input
