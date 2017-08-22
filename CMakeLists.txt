cmake_minimum_required(VERSION 3.5)
project(Obsidian2D)

set(CMAKE_CXX_FLAGS "-lxcb")

set(CMAKE_BUILD_TYPE Debug)

set(CORE_MAIN_FILE src/main.cpp)

include_directories(include include/)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(COMMON_COMPILE_FLAGS "-Wall -Wpedantic -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-strict-aliasing -fno-builtin-memcmp")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${COMMON_COMPILE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} -std=c++17 -fno-rtti -fno-exceptions -O3")
    if (UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    endif()
endif()

find_library(Vulkan_LIBRARY NAMES vulkan HINTS  "${CMAKE_SOURCE_DIR}/include/Obsidian2d/Renderer/vulkan" REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

	set(FALLBACK_CONFIG_DIRS "/etc/xdg" CACHE STRING
			"Search path to use when XDG_CONFIG_DIRS is unset or empty or the current process is SUID/SGID. Default is freedesktop compliant.")
	set(FALLBACK_DATA_DIRS "/usr/local/share:/usr/share" CACHE STRING
			"Search path to use when XDG_DATA_DIRS is unset or empty or the current process is SUID/SGID. Default is freedesktop compliant.")

    add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
    include(FindPkgConfig)
	option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)
endif()

file(GLOB_RECURSE HEADER_FILES include/*)

set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(core ${CORE_MAIN_FILE} ${HEADER_FILES})
target_link_libraries(core ${Vulkan_LIBRARY})
