#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#include "elf_bin.h"

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
    header.e_shnum = 5;
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

// Write the symbol table
void elf_write_symtable(FILE *file)
{
    // The section header
    Elf64_Shdr header;

    header.sh_name = 11;		/* Section name (string tbl index) */
    header.sh_type = SHT_SYMTAB;		/* Section type */
    header.sh_flags = 0;		/* Section flags */
    header.sh_addr = 0;		/* Section virtual addr at execution */
    header.sh_offset = 417;		/* Section file offset */
    header.sh_size = sizeof(Elf64_Sym) * 4;		/* Section size in bytes */
    header.sh_link = 3;		/* Link to another section */
    header.sh_info = 3;		/* Additional section information */
    header.sh_addralign = 8;		/* Section alignment */
    header.sh_entsize = sizeof(Elf64_Sym);		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
}

void elf_write_symbols(FILE *file)
{
    // The symbols
    Elf64_Sym symbol;
    
    // The null symbol
    symbol.st_name = 0;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = STN_UNDEF;
    symbol.st_value = 0;
    symbol.st_size = 0;
    
    fwrite(&symbol, sizeof(symbol), 1, file);
    
    // The file symbol
    symbol.st_name = 1;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_FILE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 0;
    symbol.st_value = 0;
    symbol.st_size = 0;
    
    fwrite(&symbol, sizeof(symbol), 1, file);
    
    // The section symbol
    symbol.st_name = 0;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 4;
    symbol.st_value = 0;
    symbol.st_size = 0;
    
    fwrite(&symbol, sizeof(symbol), 1, file);
    
    // The _start symbol
    symbol.st_name = 11;
    symbol.st_info = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 4;
    symbol.st_value = 0;
    symbol.st_size = 0;
    
    fwrite(&symbol, sizeof(symbol), 1, file);
}

// Write the string table
void elf_write_strtab(FILE *file)
{
    Elf64_Shdr header;

    header.sh_name = 19;		/* Section name (string tbl index) */
    header.sh_type = SHT_STRTAB;		/* Section type */
    header.sh_flags = 0;		/* Section flags */
    header.sh_addr = 0;		/* Section virtual addr at execution */
    header.sh_offset = 513;		/* Section file offset */
    header.sh_size = 18;		/* Section size in bytes */
    header.sh_link = 0;		/* Link to another section */
    header.sh_info = 0;		/* Additional section information */
    header.sh_addralign = 1;		/* Section alignment */
    header.sh_entsize = 0;		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
}

void elf_write_strtable(FILE *file)
{
    fputc(0, file);  
    fputs("first.asm", file);
    fputc(0, file);  
    fputs("_start", file);
    fputc(0, file);
}

// Write the .text header
void elf_write_text(FILE *file, int size)
{
    Elf64_Shdr header;

    header.sh_name = 27;		/* Section name (string tbl index) */
    header.sh_type = SHT_PROGBITS;		/* Section type */
    header.sh_flags = SHF_ALLOC | SHF_EXECINSTR;		/* Section flags */
    header.sh_addr = 0;		/* Section virtual addr at execution */
    header.sh_offset = 531;		/* Section file offset */
    header.sh_size = size;		/* Section size in bytes */
    header.sh_link = 0;		/* Link to another section */
    header.sh_info = 0;		/* Additional section information */
    header.sh_addralign = 16;		/* Section alignment */
    header.sh_entsize = 0;		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
}


