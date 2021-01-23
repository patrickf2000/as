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

// A utility function to add all the needed headers
void elf_add_all_sections(Elf *builder)
{
    // Create the section string header
    elf_add_section(".shstrtab", builder);
    
    // Create the string table (which other sections use)
    elf_add_section(".strtab", builder);
    
    // Create the symbol table header
    elf_add_section(".symtab", builder);
    
    // Create the .rela.text section
    elf_add_section(".rela.text", builder);
    
    // Create the data section
    elf_add_section(".data", builder);
    
    // Create the code (.text) section
    elf_add_section(".text", builder);
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

// Writes the whole thing out
// Call this function, then write your .data and .text in that order
void elf_write(Elf *builder, FILE *file)
{
    SHList *headers = builder->section_list;
    
    // Write the ELF header
    unsigned char e_ident[16] = {
        0x7F, 'E', 'L', 'F', 2, 1, 1, 3,
        1, 0, 0, 0, 0, 0, 0, 0
    };

    Elf64_Ehdr header;
    memcpy(header.e_ident, e_ident, 16);
    
    header.e_type = 1;
    header.e_machine = EM_X86_64;
    header.e_version = 1;
    header.e_entry = 0;
    header.e_phoff = 0;
    header.e_shoff = 64;
    header.e_flags = 0;
    header.e_ehsize = 64;
    header.e_phentsize = 0;
    header.e_phnum = 0;
    header.e_shentsize = 64;
    header.e_shnum = headers->size;
    header.e_shstrndx = 1;
    
    fwrite(&header, sizeof(Elf64_Ehdr), 1, file);
    
    // Write the section headers
    for (int i = 0; i<headers->size; i++)
    {
        Elf64_Shdr *current = headers->headers[i];
        fwrite(current, sizeof(Elf64_Shdr), 1, file);
    }
    
    // Write the ELF-specific data
    str_list_write(builder->shstrlist, true, file);
    str_list_write(builder->strtab, true, file);
    
    symtab_write(builder->symtab, file);
    
    rela_tab_write(builder->rela_tab, file);
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

