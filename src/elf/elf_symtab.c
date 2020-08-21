// Handles the symbol table
#include <stdlib.h>
#include <string.h>

#include <elf/elf_bin.h>

// Write the symbol table
int elf_header_symtab(FILE *file, int name_pos, int offset, int count, int start_pos)
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
    header.sh_info = start_pos;		/* Additional section information */
    header.sh_addralign = 8;		/* Section alignment */
    header.sh_entsize = sizeof(Elf64_Sym);		/* Entry size if section holds table */

    fwrite(&header, sizeof(header), 1, file);
    
    return offset + size;
}

// Write the symbol table
void elf_write_symtab(FILE *file, Elf64_SymTab *symtab)
{
    fwrite(symtab->symbols, sizeof(Elf64_Sym), symtab->size, file);
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
    symbol.st_shndx = 4;
    symbol.st_value = 0;
    symbol.st_size = 0;
    symbols[2] = symbol;
    
    // The section symbol (text)
    symbol.st_name = 0;
    symbol.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 5;
    symbol.st_value = 0;
    symbol.st_size = 0;
    symbols[3] = symbol;
    
    // Build and return the table
    Elf64_SymTab *symtab = malloc(sizeof(Elf64_SymTab));
    symtab->symbols = symbols;
    symtab->size = 4;
    
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
    symbol.st_shndx = 4;
    symbol.st_value = value;
    symbol.st_size = 0;
    table->symbols[size-1] = symbol;
}

// Adds the start entry
int elf_add_start_symbol(Elf64_SymTab *table)
{
    // Reallocate
    int size = table->size + 1;
    table->symbols = realloc(table->symbols, sizeof(Elf64_Sym) * size);
    table->size = size;
    
    // Add the symbol
    Elf64_Sym symbol;
    symbol.st_name = 11;
    symbol.st_info = ELF64_ST_INFO(STB_GLOBAL, STT_FUNC);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = 5;
    symbol.st_value = 0;
    symbol.st_size = 0;
    table->symbols[size-1] = symbol;
    
    return size - 1;
}

int get_str_pos(char *values, int last_pos)
{
    if (last_pos == 0)
        return 0;
        
    int pos = 0;
        
    for (int i = 0; i<strlen(values); i++)
    {
        if (values[i] == '|')
        {
            if (pos == last_pos)
                return i;
            else
                ++pos;
        }
    }
    
    return 0;
}

// Insert data symbols
// This involes adding everything to the symbol table and calculating offsets
char *elf_insert_data_symbols(Elf64_SymTab *symtab, SymbolTable *dataPos, char *names, char *values, char *strtab)
{
    int length = strlen(names);
    int old_length = strlen(strtab);
    int index = old_length;
    int c_index = 0;
    int last_pos = 0;
    int last_str_pos = -1;
    
    char *buf = calloc(100, sizeof(char));
    strtab = realloc(strtab, sizeof(char) + old_length + length);
    
    for (int i = 0; i<length; i++)
    {
        if (names[i] == '|')
        {
            strtab[index] = '|';
            ++index;
            
            int pos = get_str_pos(values, last_str_pos);
            sym_table_add(dataPos, buf, pos);
            
            elf_add_data_symbol(symtab, last_pos, pos);
            last_pos = index;
            ++last_str_pos;
            
            c_index = 0;
            memset(buf, 0, 100);
        }
        else if (i + 1 == length)
        {
            strtab[index] = names[i];
            buf[c_index] = names[i];
            
            int pos = get_str_pos(values, last_str_pos);
            sym_table_add(dataPos, buf, pos);
            
            elf_add_data_symbol(symtab, last_pos, pos);
            last_pos = index;
        }
        else
        {
            strtab[index] = names[i];
            ++index;
            
            buf[c_index] = names[i];
            ++c_index;
        }
    }
    
    free(buf);
    
    return strtab;
}
