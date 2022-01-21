cmake_minimum_required (VERSION 3.10)

project (vkSampleFramework)

set(FRAMEWORK_DIR ../../framework)
add_subdirectory( ${FRAMEWORK_DIR} framework )
add_subdirectory(../../samples/empty/ samples/empty)
