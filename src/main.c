#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <elf.h>

#include "elf_builder.h"
#include "str_list.h"

/* General assembly process

1) Either build or collect the size of each data section
2) Build the section headers, calculating the offset in the process
3) Write all the section headers
4) Write the data

The ELF file must minimally have this format:
1) Header
2) Null section header
3) Section string header
*/
int main(int argc, char *argv[])
{
    FILE *file;
    file = fopen("a.out", "wb");
    
    // Create the ELF builder
    Elf *builder = elf_builder_create("first.asm");
    
    //////////////////////////////////////////////////////////////////////
    // Pass 1
    
    // The data size
    // TODO: Run pass 1 on the data section here
    
    StrList *data_section = str_list_create();
    str_list_add(data_section, "Hello!");
    
    builder->data_size = str_list_size(data_section) - 1;
    
    // The code size
    // TODO: Run pass 1 on the code section here
    builder->code_size = 13;
    
    // The symbol table
    elf_add_global_function("main", 0, builder);
    
    int data_offset = 0;
    elf_add_data_symbol("STR", 3, data_offset, builder);
    
    elf_add_extern_function("puts", 8, builder);
    
    //////////////////////////////////////////////////////////////////////
    // Pass 2
    
    // Write
    elf_add_all_sections(builder);
    elf_write(builder, file);
    
    // .data
    str_list_write(data_section, false, file);
    
    // TODO: Assembler pass 2 here
    /*fputc(0xB8, file);
    int code = 60;
    fwrite(&code, sizeof(int), 1, file);
    
    fputc(0xBF, file);
    code = 5;
    fwrite(&code, sizeof(int), 1, file);
    
    fputc(0x0F, file);
    fputc(0x05, file);*/
    
    fputc(0x48, file);
    fputc(0xC7, file); fputc(0xC7, file);
    int code = 0;
    fwrite(&code, sizeof(int), 1, file);
    
    fputc(0xE8, file);
    fwrite(&code, sizeof(int), 1, file);
    
    fputc(0xC3, file);
    
    // Cleanup
    str_list_destroy(data_section);
    elf_builder_destroy(builder);
    
    fclose(file);

    return 0;
}
