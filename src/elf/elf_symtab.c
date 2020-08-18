// Handles the symbol table

#include <elf/elf_bin.h>

// Write the symbol table
int elf_write_symtable(FILE *file, int offset)
{
    int size = sizeof(Elf64_Sym) * 4;

    // The section header
    Elf64_Shdr header;

    header.sh_name = 11;		/* Section name (string tbl index) */
    header.sh_type = SHT_SYMTAB;		/* Section type */
    header.sh_flags = 0;		/* Section flags */
    header.sh_addr = 0;		/* Section virtual addr at execution */
    header.sh_offset = offset;		/* Section file offset */
    header.sh_size = size;		/* Section size in bytes */
    header.sh_link = 3;		/* Link to another section */
    header.sh_info = 3;		/* Additional section information */
    header.sh_addralign = 8;		/* Section alignment */
    header.sh_entsize = sizeof(Elf64_Sym);		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
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
