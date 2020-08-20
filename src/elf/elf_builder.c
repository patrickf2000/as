#include <string.h>
#include <stdlib.h>

#include <elf/elf_builder.h>
#include <elf/elf_bin.h>
#include <utils/str_table.h>

// Builds a relocatable object file
void build_obj(FILE *file, DataInfo *data, int code_size)
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
    
    Elf64_SymTab *symtab = elf_generate_symtab();
    
    // Add the data symbols to the table
    int length = strlen(data->names);
    char *buf = calloc(length, sizeof(char));
    int index = 0;
    
    for (int i = 1; i<length; i++)
    {
        if (data->names[i] == '|' || i + 1 == length)
        {
            int pos = str_table_add(buf, strtab);
            elf_add_data_symbol(symtab, pos, 0);
        
            index = 0;
            memset(buf, 0, length);
        }
        else
        {
            buf[index] = data->names[i];
            ++index;
        }
    }
    
    free(buf);
    
    // Build the rest
    int offset = 8 * 64;
        
    elf_write_header(file);
    elf_write_null_header(file);
        
    offset = elf_header_shstrtab(file, shstrtab_name, offset, shstrtable);
    offset = elf_header_symtab(file, symtab_name, offset, symtab->size);
    offset = elf_header_strtab(file, strtab_name, offset, strtab);
    offset = elf_header_rela_text(file, rela_text_name, offset, data->values);
    offset = elf_header_sec_data(file, data_name, offset, data->values);
    offset = elf_header_text(file, text_name, offset, code_size);
        
    str_table_write(file, shstrtable);
    elf_write_symtab(file, symtab);
    str_table_write(file, strtab);
    elf_write_rela_text(file, data->values);
    str_table_write(file, data->values);
    
    free(shstrtable);
    free(strtab);
}

// Builds a simple ELF with only a header and program header
void build_simple_exe(FILE *file)
{
    //TODO-> Do this before merge
}
