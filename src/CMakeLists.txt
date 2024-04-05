add_executable(procgen main.cpp inputcodes.h)

add_subdirectory(extern)
add_subdirectory(platform)
add_subdirectory(rendering)
add_subdirectory(graphics)
add_subdirectory(services)
target_sources(procgen PRIVATE application.cpp application.hpp)

target_link_libraries(procgen SDL3::SDL3 glm::glm nlohmann_json::nlohmann_json)
target_compile_options(procgen PRIVATE -Wall -Wextra -Wpedantic -g)

