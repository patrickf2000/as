#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include <elf/elf_bin.h>

// A utility function for building a string table
int elf_add_strtab(char *str, char *strtab)
{
    int old_start = strlen(strtab);
    
    int new_len = strlen(str) + strlen(strtab) + 1;
    strtab = realloc(strtab, sizeof(char)*new_len);
    
    strtab[old_start] = '|';   
    
    strcat(strtab, str);
    
    return old_start + 1;
}

// A utility function to write a string table
void elf_write_strtable(FILE *file, char *table)
{
    for (int i = 0; i<strlen(table); i++)
    {
        if (table[i] == '|') fputc(0, file);
        else fputc(table[i], file);
    }
    
    fputc(0, file);
}

//Write the section header string table data
int elf_header_shstrtab(FILE *file, int name_pos, int offset, char *table)
{
    int size = strlen(table) + 1;

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
int elf_header_strtab(FILE *file, int name_pos, int offset, char *table)
{
    int size = strlen(table) + 1;

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
