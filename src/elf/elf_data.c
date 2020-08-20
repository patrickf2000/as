#include <string.h>

#include <elf/elf_bin.h>

// Generates a header for the .rela.text section
int elf_header_rela_text(FILE *file, int name_pos, int offset, char *data)
{
    int size = sizeof(Elf64_Rela) * 1;
    Elf64_Shdr header;

    header.sh_name = name_pos;		                // Section name (string tbl index)
    header.sh_type = SHT_RELA;    		            // Section type
    header.sh_flags = 0;	                        // Section flags
    header.sh_addr = 0;		                        // Section virtual addr at execution
    header.sh_offset = offset;		                // Section file offset
    header.sh_size = size;		                    // Section size in bytes
    header.sh_link = 2;		                        // Link to symbol table
    header.sh_info = 6;		                        // Link to text
    header.sh_addralign = 8;		                // Section alignment
    header.sh_entsize = sizeof(Elf64_Rela);         // Entry size if section holds table

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
}

// Writes the .rela.text section
void elf_write_rela_text(FILE *file, char *data)
{
    Elf64_Rela rela;
    rela.r_offset = 0;
    rela.r_info = ELF64_R_INFO(6,1);
    rela.r_addend = 0;
    
    fwrite(&rela, sizeof(rela), 1, file);
}

// Generates a header for the .data section
int elf_header_sec_data(FILE *file, int name_pos, int offset, char *data)
{
    int size = strlen(data);
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
