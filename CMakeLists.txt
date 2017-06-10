cmake_minimum_required(VERSION 3.7)
project(Obsidian2D_CORE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -lvulkan -lxcb")

set(CMAKE_BUILD_TYPE Debug)

set(CORE_MAIN_FILE src/main.cpp src/XcbWindow.cpp)

include_directories(include)

#@TODO include GLS libs to WIN32
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers")
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} -fno-strict-aliasing -fno-builtin-memcmp")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${COMMON_COMPILE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} -std=c++11 -fno-rtti")
    if (UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    endif()
endif()
set (BUILDTGT_DIR build)
set (BINDATA_DIR Bin)
set (LIBSOURCE_DIR Lib)

option(CUSTOM_GLSLANG_BIN_ROOT "Use the user defined GLSLANG_BINARY_ROOT" OFF)
option(CUSTOM_SPIRV_TOOLS_BIN_ROOT "Use the user defined SPIRV_TOOLS_BINARY_ROOT" OFF)

set(GLSLANG_BINARY_ROOT "${CMAKE_SOURCE_DIR}/external/glslang" CACHE STRING "User defined path to the glslang binaries for this project")
set(SPIRV_TOOLS_BINARY_ROOT "${CMAKE_SOURCE_DIR}/external/spirv-tools" CACHE STRING "User defined path to the SPIRV-Tools binaries for this project")

# Define a variable for a default root location to the gslang, SPIRV-Tools and other external sources and cache it to allow the user to customize it as needed
set(EXTERNAL_SOURCE_ROOT "${CMAKE_SOURCE_DIR}/external" CACHE STRING "Root path to external sources such as glslang and SPIRV-Tools")

#non windows
if(CUSTOM_GLSLANG_BIN_ROOT)
    set (GLSLANG_SEARCH_PATH "${GLSLANG_BINARY_ROOT}/install/lib"
            "${GLSLANG_BINARY_ROOT}/glslang"
            "${GLSLANG_BINARY_ROOT}/glslang/OSDependent/Unix"
            "${GLSLANG_BINARY_ROOT}/OGLCompilersDLL"
            "${GLSLANG_BINARY_ROOT}/SPIRV"
            "${GLSLANG_BINARY_ROOT}/hlsl"
            "${GLSLANG_BINARY_ROOT}/StandAlone")
else()
    set (GLSLANG_SEARCH_PATH "${EXTERNAL_SOURCE_ROOT}/glslang/${BUILDTGT_DIR}/install/lib" "${CMAKE_SOURCE_DIR}/../x86_64/lib/glslang" )
endif()

if(CUSTOM_SPIRV_TOOLS_BIN_ROOT)
    set (SPIRV_TOOLS_SEARCH_PATH "${SPIRV_TOOLS_BINARY_ROOT}/source" )
else()
    set (SPIRV_TOOLS_SEARCH_PATH "${EXTERNAL_SOURCE_ROOT}/spirv-tools/${BUILDTGT_DIR}/source" "${CMAKE_SOURCE_DIR}/../x86_64/lib/spirv-tools" )
endif()

find_program(GLSLANG_VALIDATOR NAMES glslangValidator
        HINTS "${CMAKE_SOURCE_DIR}/external/glslang/${BUILDTGT_DIR}/install/bin"
        "${GLSLANG_BINARY_ROOT}/StandAlone"
        "${PROJECT_SOURCE_DIR}/external/${BINDATA_DIR}")

find_path(GLSLANG_SPIRV_INCLUDE_DIR SPIRV/spirv.hpp HINTS "${EXTERNAL_SOURCE_ROOT}/glslang"
        "${CMAKE_SOURCE_DIR}/../glslang"
        DOC "Path to SPIRV/spirv.hpp")

find_path(SPIRV_TOOLS_INCLUDE_DIR spirv-tools/libspirv.h HINTS "${EXTERNAL_SOURCE_ROOT}/spirv-tools/include"
        "${EXTERNAL_SOURCE_ROOT}/SPIRV-Tools/include"
        "${CMAKE_SOURCE_DIR}/../spirv-tools/include"
        "${CMAKE_SOURCE_DIR}/../SPIRV-Tools/include"
        "${EXTERNAL_SOURCE_ROOT}/source/spirv-tools/external/include"
        DOC "Path to spirv-tools/libspirv.h")

find_library(GLSLANG_LIB NAMES glslang
        HINTS ${GLSLANG_SEARCH_PATH} )

find_library(OGLCompiler_LIB NAMES OGLCompiler
        HINTS ${GLSLANG_SEARCH_PATH} )

find_library(OSDependent_LIB NAMES OSDependent
        HINTS ${GLSLANG_SEARCH_PATH} )

find_library(HLSL_LIB NAMES HLSL
        HINTS ${GLSLANG_SEARCH_PATH} )

find_library(SPIRV_LIB NAMES SPIRV
        HINTS ${GLSLANG_SEARCH_PATH} )

find_library(SPIRV_REMAPPER_LIB NAMES SPVRemapper
        HINTS ${GLSLANG_SEARCH_PATH} )

find_library(SPIRV_TOOLS_LIB NAMES SPIRV-Tools
        HINTS ${SPIRV_TOOLS_SEARCH_PATH} )

set (GLSLANG_LIBRARIES ${GLSLANG_LIB} ${OGLCompiler_LIB} ${OSDependent_LIB} ${HLSL_LIB} ${SPIRV_LIB} ${SPIRV_REMAPPER_LIB})
set (SPIRV_TOOLS_LIBRARIES ${SPIRV_TOOLS_LIB})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(FALLBACK_CONFIG_DIRS "/etc/xdg" CACHE STRING
			"Search path to use when XDG_CONFIG_DIRS is unset or empty or the current process is SUID/SGID. Default is freedesktop compliant.")
	set(FALLBACK_DATA_DIRS "/usr/local/share:/usr/share" CACHE STRING
			"Search path to use when XDG_DATA_DIRS is unset or empty or the current process is SUID/SGID. Default is freedesktop compliant.")

	include(FindPkgConfig)
	option(BUILD_WSI_XCB_SUPPORT "Build XCB WSI support" ON)
	option(BUILD_WSI_XLIB_SUPPORT "Build Xlib WSI support" ON)
	option(BUILD_WSI_WAYLAND_SUPPORT "Build Wayland WSI support" ON)
	option(BUILD_WSI_MIR_SUPPORT "Build Mir WSI support" OFF)

	if (NOT OBSIDIAN2D_WSI_SELECTION)
		set(OBSIDIAN2D_WSI_SELECTION "XCB")
	endif()

	if (OBSIDIAN2D_WSI_SELECTION STREQUAL "XCB")
		if (NOT BUILD_WSI_XCB_SUPPORT)
			message( FATAL_ERROR "Selected XCB for demos build but not building Xcb support" )
		endif()
		include_directories(${XCB_INCLUDE_DIRS})
		link_libraries(${XCB_LIBRARIES})
		add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
	elseif(OBSIDIAN2D_WSI_SELECTION STREQUAL "XLIB")
		if (NOT BUILD_WSI_XLIB_SUPPORT)
			message( FATAL_ERROR "Selected XLIB for demos build but not building Xlib support" )
		endif()
		include_directories(${X11_INCLUDE_DIR})
		link_libraries(${X11_LIBRARIES})
		add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
	elseif(OBSIDIAN2D_WSI_SELECTION STREQUAL "WAYLAND")
		if (NOT BUILD_WSI_WAYLAND_SUPPORT)
			message( FATAL_ERROR "Selected Wayland for demos build but not building Wayland support" )
		endif()
		include_directories(${WAYLAND_CLIENT_INCLUDE_DIR})
		link_libraries(${WAYLAND_CLIENT_LIBRARIES})
		add_definitions(-DVK_USE_PLATFORM_WAYLAND_KHR)
	elseif(OBSIDIAN2D_WSI_SELECTION STREQUAL "MIR")
		if (NOT BUILD_WSI_MIR_SUPPORT)
			message( FATAL_ERROR "Selected MIR for demos build but not building Mir support" )
		endif()
		add_definitions(-DVK_USE_PLATFORM_MIR_KHR)
		include_directories(${MIR_INCLUDE_DIR})
		# TODO - Add Mir support
	elseif(OBSIDIAN2D_WSI_SELECTION STREQUAL "DISPLAY")
		add_definitions(-DVK_USE_PLATFORM_DISPLAY_KHR)
	else()
		message( FATAL_ERROR "Unrecognized value for OBSIDIAN2D_WSI_SELECTION: ${OBSIDIAN2D_WSI_SELECTION}" )
	endif()
endif()

set(UTIL_FILES include/Obsidian2D/Util/Loggable.h)
set(CORE_FILES include/Obsidian2D/Core/Engine.h)
set(RENDERER_FILES
		include/Obsidian2D/Renderer/VulkanInfo.h
		include/Obsidian2D/Renderer/Window.h
		include/Obsidian2D/Renderer/XcbWindow.h
		include/Obsidian2D/Renderer/Shaders.h include/Obsidian2D/Renderer/Pipeline.h include/Obsidian2D/Renderer/Util.h)

set(EXECUTABLE_OUTPUT_PATH bin)

set (PTHREAD "pthread")

add_executable(core ${CORE_MAIN_FILE} ${CORE_FILES} ${UTIL_FILES} ${RENDERER_FILES} )
target_link_libraries(core ${GLSLANG_LIBRARIES} ${PTHREAD})
