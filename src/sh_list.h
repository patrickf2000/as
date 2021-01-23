#pragma once

#include <elf.h>

typedef struct
{
    Elf64_Shdr **headers;
    int size;
    int max;
}
SHList;

SHList *sh_list_create();
void sh_create_strtab(SHList *list, int offset, int size, int name_pos);
void sh_create_symtab(SHList *list, int offset, int size, int name_pos, int start_pos);
void sh_create_rela_text(SHList *list, int offset, int size, int name_pos);
void sh_create_data(SHList *list, int offset, int size, int name_pos);
void sh_create_text(SHList *list, int offset, int size, int name_pos);
void sh_list_destroy(SHList *list);
