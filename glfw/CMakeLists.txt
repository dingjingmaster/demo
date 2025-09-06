cmake_minimum_required(VERSION 4.0)
project(glfw C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig)

pkg_check_modules(GLFW REQUIRED glfw3)
#pkg_check_modules()

include_directories(${CMAKE_SOURCE_DIR}/deps)

add_subdirectory(deps)

add_subdirectory(examples)
