#pragma once

#include <type_traits>

template <typename E>
constexpr auto to_underlying(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

enum class ActionSet : uint32_t {
	MAIN
};

enum class Action : uint32_t {
	QUIT
};

enum class State : uint32_t {
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
};

enum class Range : uint32_t {
	CameraX,
	CameraY
};


