project (demo1)
cmake_minimum_required(VERSION 3.5)


IF (CMAKE_SYSTEM_NAME MATCHES "Linux") 
    MESSAGE(STATUS "current platform: Linux ") 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows") 
    MESSAGE(STATUS "current platform: Windows") 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin") 
    MESSAGE(STATUS "current platform: Mac OS X") 
ELSE () 
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}") 
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux") 

cmake_host_system_information(RESULT R1 QUERY OS_NAME) 
MESSAGE(STATUS "OS NAME: ${R1}") 

cmake_host_system_information(RESULT R2 QUERY OS_RELEASE) 
MESSAGE(STATUS "OS RELEASE: ${R2}") 

cmake_host_system_information(RESULT R3 QUERY DISTRIB_NAME) 
MESSAGE(STATUS "OS name: ${R3}") 
