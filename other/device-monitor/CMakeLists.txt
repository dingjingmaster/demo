cmake_minimum_required(VERSION 3.15)
project(device-monitor C)

set(CMAKE_C_STANDARD 11)

find_package (PkgConfig)

pkg_check_modules(GLIB REQUIRED gio-2.0)

add_executable(device-monitor
        main.c
        utils.c utils.h
        device-monitor.c device-monitor.h
)
target_link_libraries(device-monitor PUBLIC ${GLIB_LIBRARIES})
target_include_directories(device-monitor PUBLIC ${GLIB_INCLUDE_DIRS})