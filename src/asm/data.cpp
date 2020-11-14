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