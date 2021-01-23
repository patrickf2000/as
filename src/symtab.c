#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "symtab.h"

// Creates a symbol table
SymTab *symtab_create()
{
    SymTab *symtab = (SymTab *)malloc(sizeof(SymTab));
    symtab->table = (Elf64_Sym **)malloc(sizeof(Elf64_Sym *) * 20);
    symtab->size = 4;
    symtab->max = 20;
    
    // The null symbol
    Elf64_Sym *sym = (Elf64_Sym *)malloc(sizeof(Elf64_Sym));
    sym->st_name = 0;
    sym->st_info = 0;
    sym->st_other = 0;
    sym->st_shndx = 0;
    sym->st_value = 0;
    sym->st_size = 0;
    symtab->table[0] = sym;
    
    // The file symbol
    sym = (Elf64_Sym *)malloc(sizeof(Elf64_Sym));
    sym->st_name = 1;
    sym->st_info = 4;       // STB_LOCAL, STT_FILE
    sym->st_other = 0;
    sym->st_shndx = SHN_ABS;
    sym->st_value = 0;
    sym->st_size = 0;
    symtab->table[1] = sym;
    
    // The data symbol
    sym = (Elf64_Sym *)malloc(sizeof(Elf64_Sym));
    sym->st_name = 0;
    sym->st_info = 3;       // Symbol table index
    sym->st_other = 0;
    sym->st_shndx = 5;      // .data index
    sym->st_value = 0;
    sym->st_size = 0;
    symtab->table[2] = sym;
    
    // The .text symbol
    sym = (Elf64_Sym *)malloc(sizeof(Elf64_Sym));
    sym->st_name = 0;
    sym->st_info = 3;           // Symbol table index
    sym->st_other = 0;
    sym->st_shndx = 6;          // .text index
    sym->st_value = 0;
    sym->st_size = 0;
    symtab->table[3] = sym;
    
    return symtab;
}

// Adds to the symbol table
void symtab_add_symbol(SymTab *symtab, RelaTab *rela_tab, int name_pos, int location, SymType type, int is_global)
{
    int scope = STB_LOCAL;
    if (is_global) scope = STB_GLOBAL;
    
    int shndx;
    
    switch (type)
    {
        case Data_Sym: shndx = 5; break;
        case Func_Dec: shndx = 6; break;
        case Func_Call: shndx = SHN_UNDEF; break;
    }

    Elf64_Sym *sym = (Elf64_Sym *)malloc(sizeof(Elf64_Sym));
    sym->st_name = name_pos;
    sym->st_info = ELF64_ST_INFO(scope, STT_NOTYPE);
    sym->st_other = ELF64_ST_VISIBILITY(STV_DEFAULT);
    sym->st_shndx = shndx;
    sym->st_value = location;
    sym->st_size = 0;
    
    if (is_global)
    {
        symtab->table[symtab->size] = sym;
        symtab->size += 1;
    }
    else
    {
        symtab->size += 1;
        int found_global = 0;
        
        for (int i = 0; i<symtab->size; i++)
        {
            Elf64_Sym *current = symtab->table[i];
            
            if (ELF64_ST_BIND(current->st_info) == STB_GLOBAL)
            {
                for (int j = symtab->size; j>=i; j--)
                {
                    symtab->table[j] = symtab->table[j-1];
                }
                
                symtab->table[i] = sym;
                
                break;
            }
        }
                
        for (int i = 0; i<rela_tab->size; i++)
        {
            Elf64_Rela *current = rela_tab->table[i];
            
            if (current->r_addend == -4)
            {
                int old_pos = ELF64_R_SYM(current->r_info);
                current->r_info = ELF64_R_INFO(old_pos + 1, 2);
            }
        }
    }
}

// Gets the position of a symbol in the table based on the name index
int symtab_get_pos(SymTab *symtab, int name_index)
{
    for (int i = 0; i<symtab->size; i++)
    {
        Elf64_Sym *current = symtab->table[i];
        if (current->st_name == name_index)
            return i;
    }
    
    return 0;
}

// Returns the size of the symbol table
int symtab_get_size(SymTab *symtab)
{
    int size = symtab->size;
    size *= sizeof(Elf64_Sym);
    return size;
}

// Returns the location of the first global symbol
int symtab_get_start(SymTab *symtab)
{
    for (int i = 0; i<symtab->size; i++)
    {
        Elf64_Sym *current = symtab->table[i];
        if (ELF64_ST_BIND(current->st_info) == STB_GLOBAL)
            return i;
    }
    
    return 0;
}

// Writes the symbol table to the file
void symtab_write(SymTab *symtab, FILE *file)
{
    for (int i = 0; i<symtab->size; i++)
    {
        Elf64_Sym *current = symtab->table[i];
        fwrite(current, sizeof(Elf64_Sym), 1, file);
    }
}

// Deallocate the symbol table
void symtab_destroy(SymTab *symtab)
{
    for (int i = 0; i<symtab->size; i++)
    {
        Elf64_Sym *current = symtab->table[i];
        if (current) free(current);
    }
    
    free(symtab->table);
    free(symtab);
}

