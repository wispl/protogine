#pragma once

#include <SDL3/SDL.h>

#include <memory>

namespace sdl {

/// Typesafe creation of SDL types:
/// https://eb2.co/blog/2014/04/c-plus-plus-14-and-sdl2-managing-resources/
template <typename Constructor, typename Destructor, typename... Arguments>
inline auto make_resource(Constructor c, Destructor d, Arguments&&... args)
{
    auto r = c(std::forward<Arguments>(args)...);
    return std::unique_ptr<std::decay_t<decltype(*r)>, decltype(d)>(r, d);
}

using SDL_System = int;

inline SDL_System* SDL_CreateSDL(Uint32 flags)
{
    auto init_status = new SDL_System;
    *init_status = SDL_Init(flags);
    return init_status;
}

inline void SDL_DestroySDL(SDL_System* init_status)
{
    delete init_status;
    SDL_Quit();
}

using sdlsystem_ptr = std::unique_ptr<SDL_System, decltype(&SDL_DestroySDL)>;
using window_ptr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;

// Initialize SDL (the returned int* contains the return value from SDL_Init)
inline sdlsystem_ptr make_sdlsystem(Uint32 flags)
{
    return make_resource(SDL_CreateSDL, SDL_DestroySDL, flags);
}

// Create a window (that contains both a SDL_Window and the destructor for SDL_Windows)
inline window_ptr make_window(const char* title, int w, int h, Uint32 flags)
{
    return make_resource(SDL_CreateWindow, SDL_DestroyWindow, title, w, h, flags);
}

// Panics and quits SDL, logging the error as well
inline void panic(const char* msg)
{
	SDL_Log("|SDL Error| %s: %s", msg, SDL_GetError());
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, msg, SDL_GetError(), NULL);
	SDL_Quit();
}

} // namespace sdl
