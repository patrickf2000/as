#include <string.h>
#include <stdlib.h>

#include <elf/elf_builder.h>
#include <elf/elf_bin.h>
#include <utils/str_table.h>
#include <utils/sym_table.h>

extern int parse(const char *path, FILE *f, int pass1, SymbolTable *st, Elf64_RelaTab *rt);

// Builds a relocatable object file
void build_obj(FILE *file, DataInfo *data)
{
    // Build the section string table
    char *shstrtable = calloc(1, sizeof(char));
    int shstrtab_name = str_table_add(".shstrtab", shstrtable);
    int symtab_name = str_table_add(".symtab", shstrtable);
    int strtab_name = str_table_add(".strtab", shstrtable);
    int rela_text_name = str_table_add(".rela.text", shstrtable);
    int data_name = str_table_add(".data", shstrtable);
    int text_name = str_table_add(".text", shstrtable);
    
    char *strtab = calloc(1,sizeof(char));
    str_table_add("first.asm", strtab);
    str_table_add("_start", strtab);
    
    // Create the symbol tables
    SymbolTable *sym_table = sym_table_init_default();
    Elf64_SymTab *symtab = elf_generate_symtab();
    
    Elf64_RelaTab *rela_tab = malloc(sizeof(Elf64_RelaTab));
    rela_tab->size = 0;
    
    // Add the symbols
    strtab = elf_insert_data_symbols(symtab, sym_table, data->names, data->values, strtab);
    int start_pos = elf_add_start_symbol(symtab);
    
    // Pass 1
    int code_size = parse("text.asm", file, 1, sym_table, rela_tab);
    int rela_size = rela_tab->size;
    
    // Build the rest
    int offset = 8 * 64;
        
    // The headers
    elf_write_header(file);
    elf_write_null_header(file);
        
    offset = elf_header_shstrtab(file, shstrtab_name, offset, shstrtable);
    offset = elf_header_symtab(file, symtab_name, offset, symtab->size, start_pos);
    offset = elf_header_strtab(file, strtab_name, offset, strtab);
    offset = elf_header_sec_data(file, data_name, offset, data->values);
    offset = elf_header_text(file, text_name, offset, code_size);
    offset = elf_header_rela_text(file, rela_text_name, offset, rela_size);
    
    // Write the first several sections    
    str_table_write(file, shstrtable);
    elf_write_symtab(file, symtab);
    str_table_write(file, strtab);
    elf_write_sec_data(file, data->values);
    
    // Write the code
    parse("text.asm", file, 0, sym_table, rela_tab);
    
    // Write the rela.text section
    elf_write_rela_text(file, rela_tab);
    
    free(shstrtable);
    free(strtab);
    free(rela_tab);
}

// Builds a simple ELF with only a header and program header
void build_simple_exe(FILE *file)
{
    //TODO-> Do this before merge
}
