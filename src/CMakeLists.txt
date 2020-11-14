cmake_minimum_required(VERSION 3.0.0)
project(asm_x86)

BISON_TARGET(parser grammar.y ${CMAKE_CURRENT_BINARY_DIR}/parser.yy.cpp)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/parser.lex.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

#set(ASM_SRC
#    asm/cmp.cpp
#    asm/data.cpp
#    asm/func.cpp
#    asm/logic.cpp
#    asm/math.cpp
#    asm/mov.cpp
#    asm/push.cpp
#    asm/utils.cpp
#)

set(ELF_SRC
    elf/elf_bin.cpp
    elf/elf_builder.cpp
    elf/elf_data.cpp
    elf/elf_strtab.cpp
    elf/elf_symtab.cpp
    elf/elf_text.cpp
)

set(MAIN_SRC
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
	main.cpp
)

add_executable(asmx86 
    ${MAIN_SRC}
#    ${ASM_SRC}
    ${ELF_SRC}
)
