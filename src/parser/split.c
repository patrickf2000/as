#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <parser/parser.h>

// This splits the input file into separate files based on section
void split_file(const char *in_path)
{
    FILE *reader = fopen(in_path, "r");
    FILE *writer = NULL;
    
    char c = 0;
    char *buf = malloc(sizeof(char)*255);
    int index = 0;
    
    int last_was_sec = 0;
    
    while ((c = fgetc(reader)) != EOF) 
    {
        if (c == ' ' || c == '\n' || c == '\t')
        {
            if (strcmp(buf, "section") == 0)
            {
                last_was_sec = 1;
            } 
            else if (last_was_sec)
            {
                int len = strlen(buf) + 2;
                char *path = malloc(sizeof(char)*len);
                
                for (int i = 1; i<strlen(buf); i++)
                    path[i-1] = buf[i];
                    
                strcat(path, ".asm");
                
                if (writer != NULL)
                    fclose(writer);
                    
                writer = fopen(path, "w");
                
                last_was_sec = 0;
            } 
            else 
            {
                if (writer != NULL)
                {
                    if (strlen(buf) > 0) 
                    {
                        fputs(buf, writer);
                        fputc(c, writer);
                    }
                }
            }
            
            memset(buf, 0, 255);
            index = 0;
        } else {
            buf[index] = c;
            ++index;
        }
    }
    
    fclose(reader);
    
    if (writer != NULL)
        fclose(writer);
        
    free(buf);
}
