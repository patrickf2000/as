#include <stdlib.h>
#include <string.h>

#include "str_list.h"

StrList *str_list_create()
{
    StrList *list = (StrList *)malloc(sizeof(StrList));
    list->strings = (char **)malloc(sizeof(char *) * 20);
    list->size = 0;
    list->max = 20;
    
    return list;
}

void str_list_add(StrList *list, char *str)
{
    list->strings[list->size] = strdup(str);
    list->size += 1;
}

int str_list_size(StrList *list)
{
    int size = 1;
    
    for (int i = 0; i<list->size; i++)
    {
        char *current = list->strings[i];
        size += strlen(current) + 1;
    }
    
    return size;
}

int str_list_get_pos(StrList *list, char *to_find)
{
    int pos = 1;
    
    for (int i = 0; i<list->size; i++)
    {
        char *current = list->strings[i];
        if (strcmp(to_find, current) == 0) return pos;
        pos += strlen(current) + 1;
    }
    
    return 0;
}

void str_list_write(StrList *list, int write_null, FILE *file)
{
    if (write_null)
        fputc(0, file);
    
    for (int i = 0; i<list->size; i++)
    {
        fputs(list->strings[i], file);
        fputc(0, file);
    }
}

void str_list_destroy(StrList *list)
{
    for (int i = 0; i<list->size; i++)
    {
        char *current = list->strings[i];
        if (current) free(current);
    }
    
    free(list->strings);
    free(list);
}

