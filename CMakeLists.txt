cmake_minimum_required(VERSION 3.7)
project(Obsidian2D_CORE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -lvulkan")

set(CORE_MAIN_FILE src/main.cpp)

include_directories(include)
include_directories($ENV{VULKAN_SDK}/include/vulkan)

set(EXECUTABLE_OUTPUT_PATH bin)
set(VULKAN_LIB $ENV{VULKAN_SDK}/include/vulkan/vulkan.h)

add_executable(core ${CORE_MAIN_FILE} ${VULKAN_LIB})