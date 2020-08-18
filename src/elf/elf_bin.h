#pragma once

#include <stdio.h>
#include <elf.h>

// In the functions that write the header, we take two parameters:
// offset -> The current location based on headers and amount of data
// data -> So we can determine size
//
// The header functions return the new location.

// The types
typedef struct
{
    Elf64_Sym *symbols;
    int size;
} Elf64_SymTab;

// The functions
void elf_write_header(FILE *file);
void elf_write_null_header(FILE *file);

void elf_add_strtab(char *str, char *strtab);
void elf_write_strtable(FILE *file, char *table);
int elf_header_shstrtab(FILE *file, int offset, char *table);
int elf_header_strtab(FILE *file, int offset, char *table);

int elf_header_symtab(FILE *file, int offset, int count);
Elf64_SymTab *elf_generate_symtab();
void elf_write_symtab(FILE *file, Elf64_SymTab *symtab);

int elf_header_text(FILE *file, int offset, int size);
