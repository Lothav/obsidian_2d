cmake_minimum_required(VERSION 3.7)
project(Obsidian2D_CORE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -lvulkan -lxcb")

set(CMAKE_BUILD_TYPE Debug)

set(CORE_MAIN_FILE src/main.cpp src/XcbWindow.cpp)

include_directories(include)

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

set(UTIL_FILES include/Obsidian2D/Renderer/VulkanInfo.h)
set(CORE_FILES include/Obsidian2D/Core/Engine.h)
set(RENDERER_FILES include/Obsidian2D/Renderer/Window.h include/Obsidian2D/Renderer/XcbWindow.h)

set(EXECUTABLE_OUTPUT_PATH bin)

add_executable(core ${CORE_MAIN_FILE} ${CORE_FILES} ${UTIL_FILES} ${RENDERER_FILES} )
