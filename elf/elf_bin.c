#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "elf.h"

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
    
    header.e_type = ET_EXEC;             //Object file type
    header.e_machine = EM_X86_64;        //Architecture
    header.e_version = 1;                //Current version
    header.e_entry = 4194424;            //Entry point
    header.e_phoff = 64;                 //Program header offset
    header.e_shoff = 0;                  //Section header offset
    header.e_flags = 0;                  //No flags
    header.e_ehsize = 64;                //Header size
    header.e_phentsize = 56;             //Program header size
    header.e_phnum = 1;                  //1 program header
    header.e_shentsize = 0;
    header.e_shnum = 0;
    header.e_shstrndx = 0;
    
    fwrite(&header, sizeof(header), 1, file);
}

// Write a program header
void elf_write_pheader(FILE *file)
{
    Elf64_Phdr program_header;
    
    program_header.p_type = PT_LOAD;        // Loadable program segment
    program_header.p_flags = PF_X | PF_R;   // Flags
    program_header.p_offset = 120;          // Offset relative to start of file (file size)
    program_header.p_vaddr = 4194424;       // Virtual start
    program_header.p_paddr = 0;             // Ignore
    program_header.p_filesz = 132;          // File size
    program_header.p_memsz = 132;           // Segment size (= file size)
    program_header.p_align = 1;
    
    fwrite(&program_header, sizeof(program_header), 1, file);
}