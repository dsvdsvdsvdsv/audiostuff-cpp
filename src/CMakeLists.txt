cmake_minimum_required(VERSION 3.25.1)

project(audiofx)

add_subdirectory(CommonIncludes)
add_subdirectory(Compressor)
add_subdirectory(Delay)
add_subdirectory(test)

set(BUILD_DIR)