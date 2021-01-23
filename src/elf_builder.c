#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "elf_builder.h"

// Creat the ELF builder
Elf *elf_builder_create(char *file_name)
{
    Elf *builder = (Elf *)malloc(sizeof(Elf));
    builder->shstrlist = str_list_create();
    builder->strtab = str_list_create();
    builder->symtab = symtab_create();
    builder->rela_tab = rela_tab_create();
    builder->section_list = sh_list_create();
    
    builder->offset = 64 * 8;
    builder->data_size = 0;
    builder->code_size = 0;
    
    // Setup the section string table
    str_list_add(builder->shstrlist, ".shstrtab");
    str_list_add(builder->shstrlist, ".symtab");
    str_list_add(builder->shstrlist, ".strtab");
    str_list_add(builder->shstrlist, ".rela.text");
    str_list_add(builder->shstrlist, ".data");
    str_list_add(builder->shstrlist, ".text");
    
    // Add the file name to the secondary string table
    str_list_add(builder->strtab, file_name);
    
    return builder;
}

// Adds a section to the table
void elf_add_section(char *name, Elf *builder)
{
    int offset = builder->offset;
    int name_pos = str_list_get_pos(builder->shstrlist, name);
    
    if (strcmp(name, ".shstrtab") == 0)
    {
        int shstrlist_size = str_list_size(builder->shstrlist);
        sh_create_strtab(builder->section_list, offset, shstrlist_size, name_pos);
        offset += shstrlist_size;
    }
    else if (strcmp(name, ".strtab") == 0)
    {
        int strtab_size = str_list_size(builder->strtab);
        sh_create_strtab(builder->section_list, offset, strtab_size, name_pos);
        offset += strtab_size;
    }
    else if (strcmp(name, ".symtab") == 0)
    {
        int symtab_size = symtab_get_size(builder->symtab);
        int symtab_start = symtab_get_start(builder->symtab);
        
        sh_create_symtab(builder->section_list, offset, symtab_size, name_pos, symtab_start);
        offset += symtab_size;
    }
    else if (strcmp(name, ".rela.text") == 0)
    {
        int rela_size = rela_tab_get_size(builder->rela_tab);
        
        sh_create_rela_text(builder->section_list, offset, rela_size, name_pos);
        offset += rela_size;
    }
    else if (strcmp(name, ".data") == 0)
    {
        sh_create_data(builder->section_list, offset, builder->data_size, name_pos);
        offset += builder->data_size;
    }
    else if (strcmp(name, ".text") == 0)
    {
        sh_create_text(builder->section_list, offset, builder->code_size, name_pos);
        offset += builder->code_size;
    }
    
    builder->offset = offset;
}

// Adds a global function declaration
void elf_add_global_function(char *name, int position, Elf *builder)
{
    str_list_add(builder->strtab, name);
    int name_pos = str_list_get_pos(builder->strtab, name);
    symtab_add_symbol(builder->symtab, builder->rela_tab, name_pos, position, Func_Dec, true);
}

// Adds a reference to an extern function (usually a function call)
void elf_add_extern_function(char *name, int position, Elf *builder)
{
    str_list_add(builder->strtab, name);
    
    int name_pos = str_list_get_pos(builder->strtab, name);
    symtab_add_symbol(builder->symtab, builder->rela_tab, name_pos, 0, Func_Call, true);
    
    int sym_puts_pos = symtab_get_pos(builder->symtab, name_pos);
    rela_tab_add_function(builder->rela_tab, position, sym_puts_pos);
}

// Adds a symbol from the .data section
void elf_add_data_symbol(char *name, int code_offset, int data_offset, Elf *builder)
{
    str_list_add(builder->strtab, name);
    
    int name_pos = str_list_get_pos(builder->strtab, "STR");
    symtab_add_symbol(builder->symtab, builder->rela_tab, name_pos, 0, Data_Sym, false);
    
    rela_tab_add_data(builder->rela_tab, code_offset, data_offset);
}

// Deallocate the ELF builder
void elf_builder_destroy(Elf *builder)
{
    str_list_destroy(builder->shstrlist);
    str_list_destroy(builder->strtab);
    symtab_destroy(builder->symtab);
    rela_tab_destroy(builder->rela_tab);
    sh_list_destroy(builder->section_list);
    
    free(builder);
}

