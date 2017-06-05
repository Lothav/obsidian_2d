cmake_minimum_required(VERSION 3.7)
project(Obsidian2D_CORE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -lvulkan -lxcb")

set(CMAKE_BUILD_TYPE Debug)

set(CORE_MAIN_FILE src/main.cpp src/instance.cpp src/window.cpp)

include_directories(include)


set(UTIL_FILES include/Obsidian2D/Util/VulkanInfo.h)
set(CORE_FILES include/Obsidian2D/Core/Engine.h)
set(RENDERER_FILES include/Obsidian2D/Renderer/instance.h include/Obsidian2D/Renderer/window.h)

set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(core ${CORE_MAIN_FILE} ${UTIL_FILES} ${RENDERER_FILES} ${CORE_FILES})