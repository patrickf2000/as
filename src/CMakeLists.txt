cmake_minimum_required(VERSION 3.0.0)
project(asm_x86)

BISON_TARGET(parser grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.c)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/parser.lex.c)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

set(ASM_SRC
    asm/cmp.c
    asm/data.c
    asm/logic.c
    asm/math.c
    asm/mov.c
    asm/push.c
    asm/utils.c
)

set(ELF_SRC
    elf/elf_bin.c
    elf/elf_shstrtab.c
)

set(PARSER_SRC
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
	main.c
    sym_table.c
)

add_executable(asmx86 ${PARSER_SRC} ${ASM_SRC} ${ELF_SRC})
