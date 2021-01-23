#pragma once

#include <stdio.h>
#include <elf.h>

#include "rela_tab.h"

typedef struct
{
    Elf64_Sym **table;
    int size;
    int max;
} SymTab;

typedef enum
{
    Data_Sym,
    Func_Dec,
    Func_Call
} SymType;

SymTab *symtab_create();
void symtab_add_symbol(SymTab *symtab, RelaTab *rela_tab, int name_pos, int location, SymType type, int is_global);
int symtab_get_pos(SymTab *symtab, int name_index);
int symtab_get_size(SymTab *symtab);
int symtab_get_start(SymTab *symtab);
void symtab_write(SymTab *symtab, FILE *file);
void symtab_destroy(SymTab *symtab);

