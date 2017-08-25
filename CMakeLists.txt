cmake_minimum_required(VERSION 3.5)
project(Obsidian2D CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_CXX_FLAGS "-std=c++14 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -Wall -Wpedantic -Wextra -fno-strict-aliasing -fno-builtin-memcmp -fno-rtti -fno-exceptions")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O3")
endif()

set(MAIN_FILE src/main.cpp)

set(EXECUTABLE_OUTPUT_PATH bin)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE HEADER_FILES extlib/**/*.h)
file(GLOB_RECURSE SOURCE_FILES extlib/**/*.cpp)

include_directories(extlib/imgui)

file(COPY assets DESTINATION ${EXECUTABLE_OUTPUT_PATH})

add_executable(game ${HEADER_FILES} ${SOURCE_FILES} ${MAIN_FILE})
target_link_libraries(game ${OPENGL_LIBRARIES})
