#pragma once

#include <stdio.h>

#include <elf/elf_bin.h>

void build_obj(FILE *file, Elf64_SymTab *symtab, int code_size);
void build_simple_exe(FILE *file);
