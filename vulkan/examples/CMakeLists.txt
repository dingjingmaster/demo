add_executable(hello-world1 hello-world1.cc)
target_link_libraries(hello-world1 PUBLIC ${GLFW_LIBRARIES} ${VULKAN_LIBRARIES})
target_include_directories(hello-world1 PUBLIC ${GLFW_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS})

add_executable(hello-world2 hello-world2.cc)
target_link_libraries(hello-world2 PUBLIC ${GLFW_LIBRARIES} ${VULKAN_LIBRARIES})
target_include_directories(hello-world2 PUBLIC ${GLFW_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS})
