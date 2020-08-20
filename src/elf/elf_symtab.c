// Handles the symbol table
#include <stdlib.h>

#include <elf/elf_bin.h>

// Write the symbol table
int elf_header_symtab(FILE *file, int name_pos, int offset, int count)
{
    int size = sizeof(Elf64_Sym) * count;

    // The section header
    Elf64_Shdr header;

    header.sh_name = name_pos;		/* Section name (string tbl index) */
    header.sh_type = SHT_SYMTAB;		/* Section type */
    header.sh_flags = 0;		/* Section flags */
    header.sh_addr = 0;		/* Section virtual addr at execution */
    header.sh_offset = offset;		/* Section file offset */
    header.sh_size = size;		/* Section size in bytes */
    header.sh_link = 3;		/* Link to another section */
    header.sh_info = 6;		/* Additional section information */
    header.sh_addralign = 8;		/* Section alignment */
    header.sh_entsize = sizeof(Elf64_Sym);		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
}

// Generate the default symbol table
Elf64_SymTab *elf_generate_symtab()
{
    // The symbol variable
    Elf64_Sym *symbols = malloc(sizeof(Elf64_Sym)*5);
    Elf64_Sym symbol;
    
    // The null symbol
    symbol.st_name = 0;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = STN_UNDEF;
    symbol.st_value = 0;
    symbol.st_size = 0;
    symbols[0] = symbol;
    
    // The file symbol
    symbol.st_name = 1;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_FILE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = SHN_ABS;
    symbol.st_value = 0;
    symbol.st_size = 0;
    symbols[1] = symbol;
    
    // The section symbol (data)
    symbol.st_name = 0;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 5;
    symbol.st_value = 0;
    symbol.st_size = 0;
    symbols[2] = symbol;
    
    // The section symbol (text)
    symbol.st_name = 0;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 6;
    symbol.st_value = 0;
    symbol.st_size = 0;
    symbols[3] = symbol;
    
    // The _start symbol
    symbol.st_name = 11;
    symbol.st_info = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 6;
    symbol.st_value = 0;
    symbol.st_size = 0;
    symbols[4] = symbol;
    
    // Build and return the table
    Elf64_SymTab *symtab = malloc(sizeof(Elf64_SymTab));
    symtab->symbols = symbols;
    symtab->size = 5;
    
    return symtab;
}

// A .data symbol to the symbol table
void elf_add_data_symbol(Elf64_SymTab *table, int name_pos, int value)
{
    // Reallocate
    int size = table->size + 1;
    table->symbols = realloc(table->symbols, sizeof(Elf64_Sym) * size);
    table->size = size;
    
    // Add the symbol
    Elf64_Sym symbol;
    symbol.st_name = name_pos;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 5;
    symbol.st_value = value;
    symbol.st_size = 0;
    table->symbols[size-1] = symbol;
}

// Write the symbol table
void elf_write_symtab(FILE *file, Elf64_SymTab *symtab)
{
    fwrite(symtab->symbols, sizeof(Elf64_Sym), symtab->size, file);
}
