project(TheHell2)
cmake_minimum_required(VERSION 2.8)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include("utils")

include_directories(${CMAKE_SOURCE_DIR})

RESET_DEPS()

add_subdirectory("External_Deps")
add_subdirectory("lib")
add_subdirectory("src")
add_subdirectory("Tests")


BUILD_DEPS()