add_executable(wave wave.c)
target_link_libraries(wave PUBLIC ${GLFW_LIBRARIES} -lm)
target_include_directories(wave PUBLIC ${GLFW_INCLUDE_DIRS})
