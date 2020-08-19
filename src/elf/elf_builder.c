#include <string.h>
#include <stdlib.h>

#include <elf/elf_builder.h>
#include <elf/elf_bin.h>

// Builds a relocatable object file
void build_obj(FILE *file, int code_size)
{
    // Build the section string table
    char *shstrtable = malloc(sizeof(char)*1);
    int shstrtab_name = elf_add_strtab(".shstrtab", shstrtable);
    int symtab_name = elf_add_strtab(".symtab", shstrtable);
    int strtab_name = elf_add_strtab(".strtab", shstrtable);
    int text_name = elf_add_strtab(".text", shstrtable);
    
    char *strtab = malloc(sizeof(char)*1);
    elf_add_strtab("first.asm", strtab);
    elf_add_strtab("_start", strtab);
    
    // Build the symbol table
    Elf64_SymTab *symtab = elf_generate_symtab();
    
    // Build the rest
    int offset = 6 * 64;
        
    elf_write_header(file);
    elf_write_null_header(file);
        
    offset = elf_header_shstrtab(file, shstrtab_name, offset, shstrtable);
    offset = elf_header_symtab(file, symtab_name, offset, symtab->size);
    offset = elf_header_strtab(file, strtab_name, offset, strtab);
    offset = elf_header_text(file, text_name, offset, code_size);
        
    elf_write_strtable(file, shstrtable);
    elf_write_symtab(file, symtab);
    elf_write_strtable(file, strtab);
}

// Builds a simple ELF with only a header and program header
void build_simple_exe(FILE *file)
{
    //TODO-> Do this before merge
}
