cmake_minimum_required(VERSION 3.0.0)
project(asm_x86)

#find_package(BISON)
#find_package(FLEX)

add_compile_options(-g)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src)

set(SRC
    src/main.c
    src/rela_tab.c
    src/sh_list.c
    src/str_list
    src/symtab.c
)

add_executable(as ${SRC})

