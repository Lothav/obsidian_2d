# Install script for directory: /home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/luizorv/Downloads/VulkanSamples/external/glslang/build/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/luizorv/Downloads/VulkanSamples/external/glslang/build/SPIRV/libSPIRV.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/luizorv/Downloads/VulkanSamples/external/glslang/build/SPIRV/libSPVRemapper.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SPIRV" TYPE FILE FILES
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/bitutils.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/spirv.hpp"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/GLSL.std.450.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/GLSL.ext.KHR.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/GlslangToSpv.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/hex_float.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/Logger.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/SpvBuilder.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/spvIR.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/doc.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/disassemble.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/GLSL.ext.AMD.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/GLSL.ext.NV.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/SPVRemapper.h"
    "/home/luizorv/Downloads/VulkanSamples/external/glslang/SPIRV/doc.h"
    )
endif()

