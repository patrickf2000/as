#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <elf.h>

#include "elf_builder.h"
#include "sh_list.h"
#include "str_list.h"
#include "symtab.h"
#include "rela_tab.h"

void elf_write_header(FILE *file, SHList *headers)
{
    // Write the ELF header
    unsigned char e_ident[16] = {
        0x7F, 'E', 'L', 'F', 2, 1, 1, 3,
        1, 0, 0, 0, 0, 0, 0, 0
    };

    Elf64_Ehdr header;
    memcpy(header.e_ident, e_ident, 16);
    
    header.e_type = 1;
    header.e_machine = EM_X86_64;
    header.e_version = 1;
    header.e_entry = 0;
    header.e_phoff = 0;
    header.e_shoff = 64;
    header.e_flags = 0;
    header.e_ehsize = 64;
    header.e_phentsize = 0;
    header.e_phnum = 0;
    header.e_shentsize = 64;
    header.e_shnum = headers->size;
    header.e_shstrndx = 1;
    
    fwrite(&header, sizeof(Elf64_Ehdr), 1, file);
    
    for (int i = 0; i<headers->size; i++)
    {
        Elf64_Shdr *current = headers->headers[i];
        fwrite(current, sizeof(Elf64_Shdr), 1, file);
    }
}

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
    
    // Create the section string header
    elf_add_section(".shstrtab", builder);
    
    // Create the string table (which other sections use)
    elf_add_section(".strtab", builder);
    
    // Create the symbol table header
    elf_add_section(".symtab", builder);
    
    // Create the data section
    elf_add_section(".data", builder);
    
    // Create the code (.text) section
    elf_add_section(".text", builder);
    
    // Create the .rela.text section
    elf_add_section(".rela.text", builder);
    
    // Write
    elf_write_header(file, builder->section_list);
    
    str_list_write(builder->shstrlist, true, file);
    str_list_write(builder->strtab, true, file);
    
    symtab_write(builder->symtab, file);
    
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
    
    rela_tab_write(builder->rela_tab, file);
    
    // Cleanup
    str_list_destroy(data_section);
    elf_builder_destroy(builder);
    
    fclose(file);

    return 0;
}
