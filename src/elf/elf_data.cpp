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

#include <elf/elf_bin.hpp>

// Generates a header for the .rela.text section
int elf_header_rela_text(FILE *file, int name_pos, int offset, int size)
{
    size *= sizeof(Elf64_Rela);
    Elf64_Shdr header;

    header.sh_name = name_pos;		                // Section name (string tbl index)
    header.sh_type = SHT_RELA;    		            // Section type
    header.sh_flags = 0;	                        // Section flags
    header.sh_addr = 0;		                        // Section virtual addr at execution
    header.sh_offset = offset;		                // Section file offset
    header.sh_size = size;		                    // Section size in bytes
    header.sh_link = 2;		                        // Link to symbol table
    header.sh_info = 5;		                        // Link to text
    header.sh_addralign = 8;		                // Section alignment
    header.sh_entsize = sizeof(Elf64_Rela);         // Entry size if section holds table

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
}

// Adds a .rela.text item to the table
void elf_rela_add(std::vector<Elf64_Rela> *table, int code_offset, int data_offset)
{
    // Create and add the symbol
    Elf64_Rela rela;
    rela.r_offset = code_offset;
    rela.r_info = ELF64_R_INFO(2,1);
    rela.r_addend = data_offset;
    table->push_back(rela);
}

void elf_rela_add_func(std::vector<Elf64_Rela> *table, int code_offset, int symtab_pos)
{
    // Create and add the symbol
    Elf64_Rela rela;
    rela.r_offset = code_offset;
    rela.r_info = ELF64_R_INFO(symtab_pos,2);
    rela.r_addend = -4;
    table->push_back(rela);
}

// Writes the .rela.text section
void elf_write_rela_text(FILE *file, std::vector<Elf64_Rela> *table) {
    if (table->size() == 0)
        return;
    
    for (int i = 0; i<table->size(); i++)
    {
        Elf64_Rela rela = table->at(i);
        fwrite(&rela, sizeof(Elf64_Rela), 1, file);
    }
}

// Generates a header for the .data section
int elf_header_sec_data(FILE *file, int name_pos, int offset, int size) {
    Elf64_Shdr header;

    header.sh_name = name_pos;		                // Section name (string tbl index)
    header.sh_type = SHT_PROGBITS;		            // Section type
    header.sh_flags = SHF_ALLOC | SHF_WRITE;	    // Section flags
    header.sh_addr = 0;		                        // Section virtual addr at execution
    header.sh_offset = offset;		                // Section file offset
    header.sh_size = size;		                    // Section size in bytes
    header.sh_link = 0;		                        // Link to another section
    header.sh_info = 0;		                        // Additional section information
    header.sh_addralign = 4;		                // Section alignment
    header.sh_entsize = 0;		                    // Entry size if section holds table

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
}

// Writes the data section
void elf_write_sec_data(FILE *file, std::vector<std::string> *data) {
    if (get_table_length(data) == 0)
        return;

    for (int i = 0; i<data->size(); i++) {
        auto current = data->at(i);
        fputs(current.c_str(), file);
        fputc(0, file);
    }
}

