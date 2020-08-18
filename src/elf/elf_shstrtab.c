// Handles the utilities for the section string table

#include <string.h>
#include <malloc.h>

#include <elf/elf_bin.h>

// Add a string into the shstrtable
void elf_add_shstrtab(char *str, char *shstrtable)
{
    int old_start = strlen(shstrtable);
    
    int new_len = strlen(str) + strlen(shstrtable) + 1;
    shstrtable = realloc(shstrtable, sizeof(char)*new_len);
    
    shstrtable[old_start] = '|';   
    
    strcat(shstrtable, str);
}

//Write the section header string table data
void elf_write_shstrtab(FILE *file)
{
    Elf64_Shdr header;

    header.sh_name = 1;		            // Section name (string tbl index)
    header.sh_type = SHT_STRTAB;		// Section type
    header.sh_flags = 0;		        // Section flags
    header.sh_addr = 0;		            // Section virtual addr at execution
    header.sh_offset = 384;		        // Section file offset
    header.sh_size = 33;		        // Section size in bytes
    header.sh_link = 0;		            // Link to another section
    header.sh_info = 0;		            // Additional section information
    header.sh_addralign = 1;		    // Section alignment
    header.sh_entsize = 0;		        // Entry size if section holds table

    fwrite(&header, sizeof(header), 1, file);
}

// Write the section header string table
void elf_write_shstrtable(FILE *file, char *table)
{
    for (int i = 0; i<strlen(table); i++)
    {
        if (table[i] == '|') fputc(0, file);
        else fputc(table[i], file);
    }
    
    fputc(0, file);
}