#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "sh_list.h"

// Create the list with a null header
SHList *sh_list_create()
{
    SHList *list = (SHList *)malloc(sizeof(SHList));
    list->headers = (Elf64_Shdr **)malloc(sizeof(Elf64_Shdr *) * 10);
    list->size = 1;
    list->max = 10;

    Elf64_Shdr *null_header = (Elf64_Shdr *)malloc(sizeof(Elf64_Shdr));
    null_header->sh_name = 0;
    null_header->sh_type = 0;
    null_header->sh_flags = 0;
    null_header->sh_addr = 0;
    null_header->sh_offset = 0;
    null_header->sh_size = 0;
    null_header->sh_link = 0;
    null_header->sh_info = 0;
    null_header->sh_addralign = 0;
    null_header->sh_entsize = 0;
    list->headers[0] = null_header;
    
    return list;
}

// Create a string header
void sh_create_strtab(SHList *list, int offset, int size, int name_pos)
{
    Elf64_Shdr *header = (Elf64_Shdr *)malloc(sizeof(Elf64_Shdr));
    header->sh_name = name_pos;
    header->sh_type = SHT_STRTAB;
    header->sh_flags = 0;
    header->sh_addr = 0;
    header->sh_offset = offset;
    header->sh_size = size;
    header->sh_link = 0;
    header->sh_info = 0;
    header->sh_addralign = 1;
    header->sh_entsize = 0;
    
    list->headers[list->size] = header;
    list->size += 1;
}

// Create the symbol table
// Start_pos == the first global value in the list
void sh_create_symtab(SHList *list, int offset, int size, int name_pos, int start_pos)
{
    Elf64_Shdr *header = (Elf64_Shdr *)malloc(sizeof(Elf64_Shdr));
    header->sh_name = name_pos;
    header->sh_type = SHT_SYMTAB;
    header->sh_flags = 0;
    header->sh_addr = 0;
    header->sh_offset = offset;
    header->sh_size = size;
    header->sh_link = 2;
    header->sh_info = start_pos;
    header->sh_addralign = 8;
    header->sh_entsize = sizeof(Elf64_Sym);
    
    list->headers[list->size] = header;
    list->size += 1;
}

// Creates the header for the relocatable object table
void sh_create_rela_text(SHList *list, int offset, int size, int name_pos)
{
    Elf64_Shdr *header = (Elf64_Shdr *)malloc(sizeof(Elf64_Shdr));
    header->sh_name = name_pos;
    header->sh_type = SHT_RELA;
    header->sh_flags = 0;
    header->sh_addr = 0;
    header->sh_offset = offset;
    header->sh_size = size;
    header->sh_link = 3;            // Link to the symbol table
    header->sh_info = 6;            // Link to the .text section
    header->sh_addralign = 8;
    header->sh_entsize = sizeof(Elf64_Rela);
    
    list->headers[list->size] = header;
    list->size += 1;
}

// Create the data section
void sh_create_data(SHList *list, int offset, int size, int name_pos)
{
    Elf64_Shdr *header = (Elf64_Shdr *)malloc(sizeof(Elf64_Shdr));
    header->sh_name = name_pos;
    header->sh_type = SHT_PROGBITS;
    header->sh_flags = SHF_WRITE | SHF_ALLOC;
    header->sh_addr = 0;
    header->sh_offset = offset;
    header->sh_size = size;
    header->sh_link = 0;
    header->sh_info = 0;
    header->sh_addralign = 4;
    header->sh_entsize = 0;
    
    list->headers[list->size] = header;
    list->size += 1;
}

// Create the code (.text) section
void sh_create_text(SHList *list, int offset, int size, int name_pos)
{
    Elf64_Shdr *header = (Elf64_Shdr *)malloc(sizeof(Elf64_Shdr));
    header->sh_name = name_pos;
    header->sh_type = SHT_PROGBITS;
    header->sh_flags = 6;
    header->sh_addr = 0;
    header->sh_offset = offset;
    header->sh_size = size;
    header->sh_link = 0;
    header->sh_info = 0;
    header->sh_addralign = 16;
    header->sh_entsize = 0;
    
    list->headers[list->size] = header;
    list->size += 1;
}

// Destroy the list and free memory
void sh_list_destroy(SHList *list)
{
    for (int i = 0; i<list->size; i++)
    {
        Elf64_Shdr *current = list->headers[i];
        if (current) free(current);
    }
    
    free(list->headers);
    free(list);
}

