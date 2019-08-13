cmake_minimum_required(VERSION 3.14)
project(opengl C)

INCLUDE_DIRECTORIES("core/glad/include" "./" "/usr/local/include/")

LINK_DIRECTORIES("/usr/local/lib64/")

SET(LIBRARY_SRC
        core/glad/src/glad.c;
        )
SET(DEMO_SRC
        main.c
        d1_window/creating_window.h
        d1_window/paint_triangle.h)

set(CMAKE_C_STANDARD 11)

add_executable(opengl ${LIBRARY_SRC} ${DEMO_SRC})
target_link_libraries(opengl dl m GLEW glfw3 GL X11 pthread Xrandr Xi)
