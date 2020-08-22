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
    elf/elf_builder.c
    elf/elf_data.c
    elf/elf_strtab.c
    elf/elf_symtab.c
    elf/elf_text.c
)

SET(PARSER_SRC
    parser/split.c
)

set(UTILS_SRC
    utils/str_table.c
    utils/sym_table.c
)

set(MAIN_SRC
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
	main.c
)

add_executable(asmx86 
    ${MAIN_SRC}
    ${ASM_SRC}
    ${ELF_SRC}
    ${PARSER_SRC}
    ${UTILS_SRC}
)
