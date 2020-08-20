#pragma once

#include <elf/elf_bin.h>

void split_file(const char *in_path);
char *generate_data(const char *in_path, Elf64_SymTab *symtab);
