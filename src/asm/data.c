#include <string.h>
#include <stdlib.h>

#include <asm/asm.h>

// Write string data to the file
void amd64_write_string(const char *str, FILE *file)
{
    int len = strlen(str) - 2;
    char *str2 = malloc(sizeof(char)*len);
    
    // Omit the quotes and check for escape sequences
    int index = 0;
    
    for (int i = 1; i<strlen(str)-1; i++) {
        if (str[i] == '\\' && str[i+1] == 'n') {
            str2[index] = '\n';
            i += 1;
            index++;
            continue;
        }
        
        str2[index] = str[i];
        index++;
    }
    
    fputs(str2, file);
}
