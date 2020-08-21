#pragma once

#include <stdio.h>
#include <elf.h>

#include <utils/sym_table.h>

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

typedef struct
{
    Elf64_Rela *symbols;
    int size;
} Elf64_RelaTab;

// The functions
void elf_write_header(FILE *file);
void elf_write_null_header(FILE *file);

int elf_header_shstrtab(FILE *file, int name_pos, int offset, char *table);
int elf_header_strtab(FILE *file, int name_pos, int offset, char *table);

int elf_header_symtab(FILE *file, int name_pos, int offset, int count, int start_pos);
void elf_write_symtab(FILE *file, Elf64_SymTab *symtab);
Elf64_SymTab *elf_generate_symtab();
void elf_add_data_symbol(Elf64_SymTab *table, int name_pos, int value);
int elf_add_start_symbol(Elf64_SymTab *table, int start_pos);
char *elf_insert_data_symbols(Elf64_SymTab *symtab, SymbolTable *dataPos, char *names, char *values, char *strtab);

int elf_header_rela_text(FILE *file, int name_pos, int offset, int size);
void elf_rela_add(Elf64_RelaTab *table, int code_offset, int data_offset);
void elf_write_rela_text(FILE *file, Elf64_RelaTab *table);
int elf_header_sec_data(FILE *file, int name_pos, int offset, char *data);
void elf_write_sec_data(FILE *file, char *data);

int elf_header_text(FILE *file, int name_pos, int offset, int size);
