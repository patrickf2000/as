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

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "elf_bin.hpp"

// Write an ELF header
void elf_write_header(FILE *file)
{
    Elf64_Ehdr header;
   
    // Start with the magic number and other info
    header.e_ident[EI_MAG0] = ELFMAG0;
    header.e_ident[EI_MAG1] = ELFMAG1;
    header.e_ident[EI_MAG2] = ELFMAG2;
    header.e_ident[EI_MAG3] = ELFMAG3;
    
    header.e_ident[4] = 2;    //64-bit objects
    header.e_ident[5] = 1;    //Little endian
    header.e_ident[6] = EV_CURRENT;
    header.e_ident[7] = ELFOSABI_LINUX;
    header.e_ident[8] = 1;
    
    for (int i = 9; i<EI_NIDENT; i++) header.e_ident[i] = 0;
    
    //header.e_type = ET_EXEC;             //Object file type
    header.e_type = ET_REL;
    header.e_machine = EM_X86_64;        //Architecture
    header.e_version = 1;                //Current version
    header.e_entry = 0;
    header.e_phoff = 0;                 //Program header offset
    header.e_shoff = 64;                  //Section header offset
    header.e_flags = 0;                  //No flags
    header.e_ehsize = 64;                //Header size
    header.e_phentsize = 0;              //Program header size
    header.e_phnum = 0;                  //1 program header
    header.e_shentsize = 64;
    header.e_shnum = 7;
    header.e_shstrndx = 1;
    
    fwrite(&header, sizeof(header), 1, file);
}

// Write the empty section header
void elf_write_null_header(FILE *file)
{
    Elf64_Shdr nullh;

    nullh.sh_name = 0;		/* Section name (string tbl index) */
    nullh.sh_type = SHT_NULL;		/* Section type */
    nullh.sh_flags = 0;		/* Section flags */
    nullh.sh_addr = 0;		/* Section virtual addr at execution */
    nullh.sh_offset = 0;		/* Section file offset */
    nullh.sh_size = 0;		/* Section size in bytes */
    nullh.sh_link = 0;		/* Link to another section */
    nullh.sh_info = 0;		/* Additional section information */
    nullh.sh_addralign = 0;		/* Section alignment */
    nullh.sh_entsize = 0;		/* Entry size if section holds table */

    fwrite(&nullh, sizeof(nullh), 1, file);
}

