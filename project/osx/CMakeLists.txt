cmake_minimum_required (VERSION 3.10)

project (vkSampleFramework)

set(FRAMEWORK_DIR ../../framework)
# Set the path for PkgConfig to find libraries.
set(ENV{PKG_CONFIG_PATH} "${CMAKE_SOURCE_DIR}/solution/glfw/src/") #:$ENV{PKG_CONFIG_PATH}")
message($ENV{PKG_CONFIG_PATH})
add_subdirectory( ${FRAMEWORK_DIR} framework )
add_subdirectory(../../samples/empty/ samples/empty)
