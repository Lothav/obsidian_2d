cmake_minimum_required(VERSION 3.5)
project(Obsidian2D CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "clang++-4.0")
set(CMAKE_CXX_FLAGS "-std=c++1z -lsfml-graphics -lsfml-window -lsfml-system -Wall -Wpedantic -Wextra -fvisibility=hidden -fno-strict-aliasing -fno-builtin-memcmp -fno-rtti -fno-exceptions -O3")

set(MAIN_FILE src/main.cpp)

set(EXECUTABLE_OUTPUT_PATH build)

file(COPY assets DESTINATION ${EXECUTABLE_OUTPUT_PATH})

add_executable(game ${MAIN_FILE})