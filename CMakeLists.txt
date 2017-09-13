cmake_minimum_required(VERSION 3.5)

project(Obsidian2D)

set(CMAKE_BUILD_TYPE Debug)

set(CORE_MAIN_FILE src/main.cpp)

include_directories(include include/)


if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(COMMON_COMPILE_FLAGS "-Wall -Wpedantic -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-strict-aliasing -fno-builtin-memcmp")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${COMMON_COMPILE_FLAGS}")
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    elseif(CMAKE_BUILD_TYPE MATCHES "Release")
        set(CMAKE_CXX_FLAGS " ${COMMON_COMPILE_FLAGS} -std=c++17 -fno-rtti -fno-exceptions -O3")
        if (UNIX)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
        endif()
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

if(NOT XCB_FIND_COMPONENTS)
    set(XCB_FIND_COMPONENTS xcb)
endif()

include(FindPackageHandleStandardArgs)
set(XCB_FOUND true)
set(XCB_INCLUDE_DIRS "")
set(XCB_LIBRARIES "")
foreach(comp ${XCB_FIND_COMPONENTS})
    # component name
    string(TOUPPER ${comp} compname)
    string(REPLACE "-" "_" compname ${compname})
    # header name
    string(REPLACE "xcb-" "" headername xcb/${comp}.h)
    # library name
    set(libname ${comp})

    pkg_check_modules(PC_${comp} QUIET ${comp})

    find_path(${compname}_INCLUDE_DIR NAMES ${headername}
            HINTS
            ${PC_${comp}_INCLUDEDIR}
            ${PC_${comp}_INCLUDE_DIRS}
            )

    find_library(${compname}_LIBRARY NAMES ${libname}
            HINTS
            ${PC_${comp}_LIBDIR}
            ${PC_${comp}_LIBRARY_DIRS}
            )

    find_package_handle_standard_args(${comp}
            FOUND_VAR ${comp}_FOUND
            REQUIRED_VARS ${compname}_INCLUDE_DIR ${compname}_LIBRARY)
    mark_as_advanced(${compname}_INCLUDE_DIR ${compname}_LIBRARY)

    list(APPEND XCB_INCLUDE_DIRS ${${compname}_INCLUDE_DIR})
    list(APPEND XCB_LIBRARIES ${${compname}_LIBRARY})

    if(NOT ${comp}_FOUND)
        set(XCB_FOUND false)
    endif()
endforeach()

list(REMOVE_DUPLICATES XCB_INCLUDE_DIRS)

add_executable(core ${CORE_MAIN_FILE} ${HEADER_FILES})
target_link_libraries(core ${XCB_LIBRARIES} ${Vulkan_LIBRARY})

#=======
#project(Obsidian2D CXX)
#
#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_COMPILER "clang++")
#
#set(CMAKE_CXX_FLAGS "-std=c++14 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -Wall -Wpedantic -Wextra -fno-strict-aliasing -fno-builtin-memcmp -fno-rtti -fno-exceptions")
#
#if (CMAKE_BUILD_TYPE MATCHES "Debug")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
#elseif(CMAKE_BUILD_TYPE MATCHES "Release")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -O3")
#endif()
#
#set(MAIN_FILE src/main.cpp)
#
#set(EXECUTABLE_OUTPUT_PATH bin)
#
#find_package(OpenGL REQUIRED)
#
#file(GLOB_RECURSE HEADER_FILES extlib/**/*.h)
#file(GLOB_RECURSE SOURCE_FILES extlib/**/*.cpp)
#
#include_directories(extlib)
#
#file(COPY assets DESTINATION ${EXECUTABLE_OUTPUT_PATH})
#
#add_executable(game ${HEADER_FILES} ${SOURCE_FILES} ${MAIN_FILE})
#target_link_libraries(game ${OPENGL_LIBRARIES})
#>>>>>>> master
