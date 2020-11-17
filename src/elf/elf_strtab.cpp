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

