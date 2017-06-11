# CMake generated Testfile for 
# Source directory: /home/luizorv/Downloads/VulkanSamples/external/spirv-tools
# Build directory: /home/luizorv/Downloads/VulkanSamples/external/spirv-tools/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(spirv-tools-copyrights "/usr/bin/python" "utils/check_copyright.py")
set_tests_properties(spirv-tools-copyrights PROPERTIES  WORKING_DIRECTORY "/home/luizorv/Downloads/VulkanSamples/external/spirv-tools")
subdirs("external")
subdirs("source")
subdirs("tools")
subdirs("test")
subdirs("examples")
