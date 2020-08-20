#include <string.h>
#include <stdlib.h>

#include <elf/elf_builder.h>
#include <elf/elf_bin.h>
#include <utils/str_table.h>

// Builds a relocatable object file
void build_obj(FILE *file, Elf64_SymTab *symtab, int code_size)
{
    // Build the section string table
    char *shstrtable = calloc(1, sizeof(char));
    int shstrtab_name = str_table_add(".shstrtab", shstrtable);
    int symtab_name = str_table_add(".symtab", shstrtable);
    int strtab_name = str_table_add(".strtab", shstrtable);
    int text_name = str_table_add(".text", shstrtable);
    
    char *strtab = calloc(1,sizeof(char));
    str_table_add("first.asm", strtab);
    str_table_add("_start", strtab);
    
    // Build the rest
    int offset = 6 * 64;
        
    elf_write_header(file);
    elf_write_null_header(file);
        
    offset = elf_header_shstrtab(file, shstrtab_name, offset, shstrtable);
    offset = elf_header_symtab(file, symtab_name, offset, symtab->size);
    offset = elf_header_strtab(file, strtab_name, offset, strtab);
    offset = elf_header_text(file, text_name, offset, code_size);
        
    str_table_write(file, shstrtable);
    elf_write_symtab(file, symtab);
    str_table_write(file, strtab);
    
    free(shstrtable);
    free(strtab);
}

// Builds a simple ELF with only a header and program header
void build_simple_exe(FILE *file)
{
    //TODO-> Do this before merge
}
