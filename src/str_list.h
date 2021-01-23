#pragma once

#include <stdio.h>

typedef struct
{
    char **strings;
    int size;
    int max;
} StrList;

StrList *str_list_create();
void str_list_add(StrList *list, char *str);
int str_list_size(StrList *list);
int str_list_get_pos(StrList *list, char *to_find);
void str_list_write(StrList *list, int write_null, FILE *file);
void str_list_destroy(StrList *list);

