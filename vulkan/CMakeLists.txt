cmake_minimum_required(VERSION 4.0)
project(vulkan)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig)

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(VULKAN REQUIRED vulkan)

add_subdirectory(examples)