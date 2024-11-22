add_executable(${PROJECT_NAME} main.cpp inputcodes.h)

add_subdirectory(extern)
add_subdirectory(platform)
add_subdirectory(rendering)
add_subdirectory(graphics)
add_subdirectory(services)
target_sources(${PROJECT_NAME} PRIVATE application.cpp application.h)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3 glm::glm nlohmann_json::nlohmann_json)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -g)

