cmake_minimum_required (VERSION 3.18)

project ("FrostEngine")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory ("Lib")
add_subdirectory ("Graphics")
add_subdirectory ("Tests")

