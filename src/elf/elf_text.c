// Handles things relating to the text section of the ELF file

#include <elf/elf_bin.h>

// Write the .text header
int elf_header_text(FILE *file, int name_pos, int offset, int size)
{
    Elf64_Shdr header;

    header.sh_name = name_pos;		/* Section name (string tbl index) */
    header.sh_type = SHT_PROGBITS;		/* Section type */
    header.sh_flags = SHF_ALLOC | SHF_EXECINSTR;		/* Section flags */
    header.sh_addr = 0;		/* Section virtual addr at execution */
    header.sh_offset = offset;		/* Section file offset */
    header.sh_size = size;		/* Section size in bytes */
    header.sh_link = 0;		/* Link to another section */
    header.sh_info = 0;		/* Additional section information */
    header.sh_addralign = 16;		/* Section alignment */
    header.sh_entsize = 0;		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
}
