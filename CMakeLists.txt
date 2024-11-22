cmake_minimum_required(VERSION 3.24)

project(protogine VERSION 0.0.1 DESCRIPTION "Prototype OpenGL Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Dependencies.cmake)

add_subdirectory(src)
