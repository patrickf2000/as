#pragma once

#include "str_list.h"
#include "sh_list.h"
#include "symtab.h"
#include "rela_tab.h"

typedef struct
{
    StrList *shstrlist;
    StrList *strtab;
    SymTab *symtab;
    RelaTab *rela_tab;
    SHList *section_list;
    
    int offset;
    int data_size;
    int code_size;
} Elf;

Elf *elf_builder_create(char *file_name);
void elf_add_section(char *name, Elf *builder);
void elf_add_all_sections(Elf *builder);
void elf_add_global_function(char *name, int position, Elf *builder);
void elf_add_extern_function(char *name, int position, Elf *builder);
void elf_add_data_symbol(char *name, int code_offset, int data_offset, Elf *builder);
void elf_write(Elf *builder, FILE *file);
void elf_builder_destroy(Elf *builder);

