// This file is part of the "as" assembler.
// Copyright (C) 2020 Patrick Flynn
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

// Handles the symbol table
#include <stdlib.h>
#include <string.h>

#include <vector>

#include <elf/elf_bin.hpp>

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
void elf_write_symtab(FILE *file, std::vector<Elf64_Sym> *symtab) {
    for (int i = 0; i<symtab->size(); i++) {
        Elf64_Sym current = symtab->at(i);
        fwrite(&current, sizeof(Elf64_Sym), 1, file);
    }
}

// Generate the default symbol table
void elf_generate_symtab(std::vector<Elf64_Sym> *symtab) {
    // The null symbol
    Elf64_Sym symbol1;
    symbol1.st_name = 0;
    symbol1.st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    symbol1.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol1.st_shndx = STN_UNDEF;
    symbol1.st_value = 0;
    symbol1.st_size = 0;
    symtab->push_back(symbol1);
    
    // The file symbol
    Elf64_Sym symbol2;
    symbol2.st_name = 1;
    symbol2.st_info = ELF64_ST_INFO(STB_LOCAL, STT_FILE);
    symbol2.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol2.st_shndx = SHN_ABS;
    symbol2.st_value = 0;
    symbol2.st_size = 0;
    symtab->push_back(symbol2);
    
    // The section symbol (data)
    Elf64_Sym symbol3;
    symbol3.st_name = 0;
    symbol3.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    symbol3.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol3.st_shndx = 4;
    symbol3.st_value = 0;
    symbol3.st_size = 0;
    symtab->push_back(symbol3);
    
    // The section symbol (text)
    Elf64_Sym symbol4;
    symbol4.st_name = 0;
    symbol4.st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION);
    symbol4.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol4.st_shndx = 5;
    symbol4.st_value = 0;
    symbol4.st_size = 0;
    symtab->push_back(symbol4);
}

// A symbol to the symbol table
// Type: 0 -> Local symbol
//       1 -> Global symbol
//       2 -> Extern symbol
int elf_add_symbol(std::vector<Elf64_Sym> *table, int name_pos, int value, int is_data, int type) {
    if (name_pos == 0)
        return -1;
    
    int index = 5;
    if (is_data) index = 4;
    
    int scope = STB_LOCAL;
    if (type == 1 || type == 2) scope = STB_GLOBAL;
    
    int shndx = index;
    if (type == 2) shndx = STN_UNDEF;
    
    // Add the symbol
    Elf64_Sym symbol;
    symbol.st_name = name_pos;
    symbol.st_info = ELF64_ST_INFO(scope, STT_NOTYPE);
    symbol.st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    symbol.st_shndx = shndx;
    symbol.st_value = value;
    symbol.st_size = 0;
    table->push_back(symbol);
    
    return table->size()-1;
}

// Sorts the symbol table so all global symbols are last
int elf_symtab_sort(std::vector<Elf64_Sym> *table) {
    int size = table->size();

    // First, allocate two tables
    std::vector<Elf64_Sym> local_symbols;
    std::vector<Elf64_Sym> global_symbols;
    
    int li = 0;    // Local symbol table index
    int gi = 0;    // Global symbol table index
    
    // Iterate through the original table and sort out the symbols
    for (int i = 0; i<size; i++) {
        Elf64_Sym current = table->at(i);
        
        if (ELF64_ST_BIND(current.st_info) == STB_GLOBAL) {
            global_symbols.push_back(current);
            ++gi;   
        } else {
            local_symbols.push_back(current);
            ++li;
        }
    }
    
    table->clear();
    
    // Put the tables back together
    int index = 0;
    int global_start = 0;
    
    // Start with the local symbols
    for (int i = 0; i<li; i++) {
        table->push_back(local_symbols.at(i));
        ++index;
    }
    
    global_start = index;
    
    // Now the global symbols
    for (int i = 0; i<gi; i++) {
        table->push_back(global_symbols.at(i));
    }
    
    // Return the start position of the globals
    return global_start;
}

int get_str_pos(char *values, int last_pos)
{
    if (last_pos == 0)
        return 0;
        
    int pos = 0;
        
    for (int i = 0; i<strlen(values); i++)
    {
        if (values[i] == '|' )
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
/*char *elf_insert_data_symbols(Elf64_SymTab *symtab, SymbolTable *dataPos, char *names, char *values, char *strtab)
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
            
            elf_add_symbol(symtab, last_pos, pos, 1, 0);
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
            
            elf_add_symbol(symtab, last_pos, pos, 1, 0);
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
}*/
