#pragma once

#include <stdio.h>
#include <elf.h>

// In the functions that write the header, we take two parameters:
// offset -> The current location based on headers and amount of data
// data -> So we can determine size
//
// The header functions return the new location.

void elf_add_strtab(char *str, char *strtab);
void elf_write_strtable(FILE *file, char *table);
void elf_write_header(FILE *file);
void elf_write_null_header(FILE *file);

int elf_write_shstrtab(FILE *file, int offset, char *table);

int elf_write_symtable(FILE *file, int offset);
void elf_write_symbols(FILE *file);

int elf_write_strtab(FILE *file, int offset, char *table);

int elf_write_text(FILE *file, int offset, int size);
