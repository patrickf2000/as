#include <string.h>
#include <stdlib.h>

#include <asm.h>

// Write string data to the file
void amd64_write_string(const char *str, FILE *file)
{
    int len = strlen(str) - 2;
    char *str2 = malloc(sizeof(char)*len);
    
    for (int i = 1; i<strlen(str)-1; i++) str2[i-1] = str[i];
    
    fputs(str2, file);
}
