#pragma once

#include <stdio.h>
#include <elf.h>

typedef struct
{
    Elf64_Rela **table;
    int size;
    int max;
} RelaTab;

RelaTab *rela_tab_create();
void rela_tab_add_data(RelaTab *table, int code_offset, int data_offset);
void rela_tab_add_function(RelaTab *table, int code_offset, int symtab_pos);
int rela_tab_get_size(RelaTab *table);
void rela_tab_write(RelaTab *table, FILE *file);
void rela_tab_destroy(RelaTab *table);
