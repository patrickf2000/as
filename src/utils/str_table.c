#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include <utils/str_table.h>

 // A utility function for building a string table
int str_table_add(char *str, char *strtab)
{
    int old_start = strlen(strtab);
    
    int new_len = strlen(str) + strlen(strtab) + 2;
    strtab = realloc(strtab, sizeof(char)*new_len);
    
    strtab[old_start] = '|';   
    
    strcat(strtab, str);
    
    return old_start + 1;
}

// A utility function to write a string table
void str_table_write(FILE *file, char *table)
{
    for (int i = 0; i<strlen(table); i++)
    {
        if (table[i] == '|') fputc(0, file);
        else fputc(table[i], file);
    }
    
    fputc(0, file);
} 
