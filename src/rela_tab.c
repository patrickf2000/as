#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "rela_tab.h"

// Creates a relocatable object table
RelaTab *rela_tab_create()
{
    RelaTab *rela_tab = (RelaTab *)malloc(sizeof(RelaTab));
    rela_tab->table = (Elf64_Rela **)malloc(sizeof(Elf64_Rela *) * 10);
    rela_tab->size = 0;
    rela_tab->max = 10;
    return rela_tab;
}

// Adds a .data reference to the table
void rela_tab_add_data(RelaTab *table, int code_offset, int data_offset)
{
    Elf64_Rela *rela = (Elf64_Rela *)malloc(sizeof(Elf64_Rela));
    rela->r_offset = code_offset;
    rela->r_info = ELF64_R_INFO(2, 1);
    rela->r_addend = data_offset;
    
    table->table[table->size] = rela;
    table->size += 1;
}

// Adds a function symbol to the table
void rela_tab_add_function(RelaTab *table, int code_offset, int symtab_pos)
{
    Elf64_Rela *rela = (Elf64_Rela *)malloc(sizeof(Elf64_Rela));
    rela->r_offset = code_offset;
    rela->r_info = ELF64_R_INFO(symtab_pos, 2);
    rela->r_addend = -4;
    
    table->table[table->size] = rela;
    table->size += 1;
}

// Returns the byte size of rela_tab
int rela_tab_get_size(RelaTab *table)
{
    int size = table->size * sizeof(Elf64_Rela);
    return size;
}

// Writes rela_tab to the file
void rela_tab_write(RelaTab *table, FILE *file)
{
    for (int i = 0; i<table->size; i++)
    {
        Elf64_Rela *current = table->table[i];
        fwrite(current, sizeof(Elf64_Rela), 1, file);
    }
}

// Destroy rela_tab
void rela_tab_destroy(RelaTab *table)
{
    for (int i = 0; i<table->size; i++)
    {
        Elf64_Rela *current = table->table[i];
        if (current) free(current);
    }
    
    free(table->table);
    free(table);
}

