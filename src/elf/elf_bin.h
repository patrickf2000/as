#pragma once

#include <stdio.h>
#include <elf.h>

void elf_write_header(FILE *file);
void elf_write_null_header(FILE *file);

void elf_add_shstrtab(char *str, char *shstrtable);
void elf_write_shstrtab(FILE *file);
void elf_write_shstrtable(FILE *file, char *table);

void elf_write_symtable(FILE *file);
void elf_write_symbols(FILE *file);

void elf_write_strtab(FILE *file);
void elf_write_strtable(FILE *file);

void elf_write_text(FILE *file, int size);
