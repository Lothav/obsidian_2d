cmake_minimum_required(VERSION 3.7)
project(Obsidian2D_CORE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -lvulkan")

set(CORE_MAIN_FILE src/main.cpp)

include_directories(include)

set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(core ${CORE_MAIN_FILE})
