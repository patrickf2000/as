// Copyright 2020 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this 
//	list of conditions and the following disclaimer in the documentation and/or 
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may 
//	be used to endorse or promote products derived from this software 
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include <utils/str_table.h>

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
