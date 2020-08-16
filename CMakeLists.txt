cmake_minimum_required(VERSION 3.0.0)
project(asm_x86)

find_package(BISON)
find_package(FLEX)

add_compile_options(-g)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src)

# The binaries
add_subdirectory(src)




