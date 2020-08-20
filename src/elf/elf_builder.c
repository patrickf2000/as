#include <string.h>
#include <stdlib.h>

#include <elf/elf_builder.h>
#include <elf/elf_bin.h>
#include <utils/str_table.h>
#include <utils/sym_table.h>

extern int parse(const char *path, FILE *f, int pass1, SymbolTable *st);

// Builds a relocatable object file
void build_obj(FILE *file, DataInfo *data)
{
    // Pass 1
    SymbolTable *sym_table = sym_table_init_default();
    int code_size = parse("text.asm", file, 1, sym_table);

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
            if (i + 1 == length)
                buf[index] = data->names[i];
            
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
        
    // The headers
    elf_write_header(file);
    elf_write_null_header(file);
        
    offset = elf_header_shstrtab(file, shstrtab_name, offset, shstrtable);
    offset = elf_header_symtab(file, symtab_name, offset, symtab->size);
    offset = elf_header_strtab(file, strtab_name, offset, strtab);
    offset = elf_header_sec_data(file, data_name, offset, data->values);
    offset = elf_header_text(file, text_name, offset, code_size);
    offset = elf_header_rela_text(file, rela_text_name, offset, data->values);
    
    // Write the first several sections    
    str_table_write(file, shstrtable);
    elf_write_symtab(file, symtab);
    str_table_write(file, strtab);
    str_table_write(file, data->values);
    
    // Write the code
    printf("Size: %d\n", code_size);
    parse("text.asm", file, 0, sym_table);
    
    // Write the buffer
    /*for (int i = 0; i<10; i++)
        fputc(0, file);*/
    
    // Write the rela.text section
    elf_write_rela_text(file, data->values);
    
    free(shstrtable);
    free(strtab);
}

// Builds a simple ELF with only a header and program header
void build_simple_exe(FILE *file)
{
    //TODO-> Do this before merge
}
