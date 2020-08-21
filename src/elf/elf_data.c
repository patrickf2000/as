#include <string.h>
#include <stdlib.h>

#include <elf/elf_bin.h>

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
void elf_rela_add(Elf64_RelaTab *table, int code_offset, int data_offset)
{
    // Resize the table
    int size = 1;
    
    if (table->size == 0)
    {
        table->symbols = malloc(sizeof(Elf64_Rela));
        table->size = size;
    }
    else
    {
        size = table->size + 1;
        table->symbols = realloc(table->symbols, sizeof(Elf64_Rela) * size);
        table->size = size;
    }
    
    // Create and add the symbol
    Elf64_Rela rela;
    rela.r_offset = code_offset;
    rela.r_info = ELF64_R_INFO(2,1);
    rela.r_addend = data_offset;
    table->symbols[size-1] = rela;
}

// Writes the .rela.text section
void elf_write_rela_text(FILE *file, Elf64_RelaTab *table)
{
    if (table->size == 0)
        return;
        
    for (int i = 0; i<table->size; i++)
    {
        Elf64_Rela rela = table->symbols[i];
        fwrite(&rela, sizeof(Elf64_Rela), 1, file);
    }
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

// Writes the data section
void elf_write_sec_data(FILE *file, char *data)
{
    if (strlen(data) == 0)
        return;

    for (int i = 1; i<strlen(data); i++)
    {
        if (data[i] == '|') fputc(0, file);
        else fputc(data[i], file);
    }
    
    fputc(0, file);
}

