cmake_minimum_required(VERSION 3.0.0)
project(assembler)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser src/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.c)
FLEX_TARGET(scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/parser.lex.c)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(asm src)

set(ASM_SRC
    asm/cmp.c
    asm/data.c
    asm/math.c
    asm/mov.c
    asm/push.c
    asm/utils.c
    asm/elf/elf_bin.c
)

set(PARSER_SRC
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
	src/main.c
    src/sym_table.c
)

add_executable(asmx86 ${PARSER_SRC} ${ASM_SRC})
