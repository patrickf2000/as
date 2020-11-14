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

#include <vector>
#include <string>

#include <elf/elf_bin.hpp>

//Write the section header string table data
int elf_header_shstrtab(FILE *file, int name_pos, int offset, int size) {
    Elf64_Shdr header;

    header.sh_name = name_pos;          // Section name (string tbl index)
    header.sh_type = SHT_STRTAB;		// Section type
    header.sh_flags = 0;		        // Section flags
    header.sh_addr = 0;		            // Section virtual addr at execution
    header.sh_offset = offset;	        // Section file offset
    header.sh_size = size;		        // Section size in bytes
    header.sh_link = 0;		            // Link to another section
    header.sh_info = 0;		            // Additional section information
    header.sh_addralign = 1;		    // Section alignment
    header.sh_entsize = 0;		        // Entry size if section holds table

    fwrite(&header, sizeof(header), 1, file);
    
    offset += size;
    return offset;
}

// Write the string table
int elf_header_strtab(FILE *file, int name_pos, int offset, int size) {
    Elf64_Shdr header;

    header.sh_name = name_pos;		/* Section name (string tbl index) */
    header.sh_type = SHT_STRTAB;		/* Section type */
    header.sh_flags = 0;		/* Section flags */
    header.sh_addr = 0;		/* Section virtual addr at execution */
    header.sh_offset = offset;		/* Section file offset */
    header.sh_size = size;		/* Section size in bytes */
    header.sh_link = 0;		/* Link to another section */
    header.sh_info = 0;		/* Additional section information */
    header.sh_addralign = 1;		/* Section alignment */
    header.sh_entsize = 0;		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
}

// Write a string table to our file
void elf_strtab_write(FILE *file, std::vector<std::string> *table) {
    fputc(0, file);
    
    for (int i = 0; i<table->size(); i++) {
        auto current = table->at(i);
        fputs(current.c_str(), file);
        fputc(0, file);
    }
}

