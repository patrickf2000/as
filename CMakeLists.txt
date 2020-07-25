cmake_minimum_required(VERSION 3.0.0)
project(assembler)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser src/grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.c)
FLEX_TARGET(scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/parser.lex.c)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(asm elf src)

set(ASM_SRC
    asm/data.c
    asm/math.c
    asm/misc.c
    asm/mov.c
    asm/push.c
)

set(ELF_SRC
    elf/elf_bin.c
)

set(PARSER_SRC
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
	src/main.c
    src/sym_table.c
)

add_executable(asm_parser ${PARSER_SRC} ${ASM_SRC} ${ELF_SRC})
#add_executable(asm ${ASM_SRC} ${ELF_SRC} src/asm_main.c)
