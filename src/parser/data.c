#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <parser/parser.h>
#include <utils/str_table.h>
#include <elf/elf_bin.h>

// Parses a string constant (removes quotes and escape sequences)
void parse_string(char *str)
{
    int length = strlen(str);
    int index = 0;
    
    char *old_str = calloc(length, sizeof(char));
    strcpy(old_str, str);
    memset(str, 0, length);
    
    for (int i = 0; i<length; i++)
    {
        if (old_str[i] == '\"')
        {
            continue;
        }
        else if (old_str[i] == '\\' && old_str[i+1] == 'n')
        {
            str[index] = '\n';
            ++i;
            ++index;
        }
        else
        {
            str[index] = old_str[i];
            ++index;
        }
    }
    
    free(old_str);
}

// Generates the .data section
DataInfo *generate_data(const char *in_path)
{
    // Set things up
    // File-specific stuff
    FILE *file = fopen(in_path, "r");
    
    // The string buffers
    char *buf = calloc(255, sizeof(char));
    char *name = calloc(255, sizeof(char));
    char *type = calloc(255, sizeof(char));
    char *val = calloc(255, sizeof(char));
    
    // Various index variables
    int index = 0;
    int part = 0;
    int in_quote = 0;
    char c = 0;
    
    //String table stuff
    DataInfo *info = calloc(1, sizeof(DataInfo));
    info->names = calloc(1, sizeof(char));
    info->values = calloc(1, sizeof(char));
    
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n' && index > 0)
        {
            int length = strlen(buf);
        
            // Copy the value
            strcpy(val, buf);
            parse_string(val);
            
            // Add to the tables
            str_table_add(name, info->names);
            str_table_add(val, info->values);
            
            // Reset all parts
            memset(buf, 0, 255);
            memset(name, 0, 255);
            memset(type, 0, 255);
            memset(val, 0, 255);
            index = 0;
            part = 0;
        }
        else if (c == '\"')
        {
            in_quote = !in_quote;
        }
        else if (c == ' ' && index > 0 && !in_quote)
        {
            switch (part)
            {
                case 0: strcpy(name, buf); break;
                case 1: strcpy(type, buf); break;
            }
            
            memset(buf, 0, 255);
            index = 0;
            ++part;
        }
        else
        {
            buf[index] = c;
            ++index;
        }
    }
    
    // Clean up
    fclose(file);
    free(buf);
    free(name);
    free(val);
    free(type);
    
    // Create the table and return
    return info;
}
