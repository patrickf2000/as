// This file is part of the "as" assembler.
// Copyright (C) 2020 Patrick Flynn
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <string.h>
#include <stdlib.h>

#include <asm/asm.hpp>

// Write string data to the file
void amd64_write_string(const char *str, FILE *file)
{
    /*int len = strlen(str) - 2;
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
    
    fputs(str2, file);*/
}
