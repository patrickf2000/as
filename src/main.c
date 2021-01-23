#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <elf.h>

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
    
    // Determine all data sizes and locations
    // Create the section header string table
    StrList *shstrlist = str_list_create();
    str_list_add(shstrlist, ".shstrtab");
    str_list_add(shstrlist, ".symtab");
    str_list_add(shstrlist, ".strtab");
    str_list_add(shstrlist, ".rela.text");
    str_list_add(shstrlist, ".data");
    str_list_add(shstrlist, ".text");
    
    int shstrlist_size = str_list_size(shstrlist);
    
    // The string table
    StrList *strtab = str_list_create();
    str_list_add(strtab, "first.asm");
    str_list_add(strtab, "main");
    str_list_add(strtab, "puts");
    str_list_add(strtab, "STR");
    
    int strtab_size = str_list_size(strtab);
    
    // The symbol table
    SymTab *symtab = symtab_create();
    
    int start_name_pos = str_list_get_pos(strtab, "main");
    symtab_add_symbol(symtab, start_name_pos, 0, Func_Dec, true);
    
    int puts_pos = str_list_get_pos(strtab, "puts");
    symtab_add_symbol(symtab, puts_pos, 0, Func_Call, true);
    
    int str_pos = str_list_get_pos(strtab, "STR");
    symtab_add_symbol(symtab, str_pos, 0, Data_Sym, false);
    
    symtab_sort(symtab);
    int symtab_size = symtab_get_size(symtab);
    int symtab_start = symtab_get_start(symtab);
    
    // The .rela.text table
    RelaTab *rela_tab = rela_tab_create();
    
    int data_offset = 0;
    rela_tab_add_data(rela_tab, 3, data_offset);
    
    puts_pos = str_list_get_pos(strtab, "puts");
    int sym_puts_pos = symtab_get_pos(symtab, puts_pos);
    rela_tab_add_function(rela_tab, 8, sym_puts_pos);
    
    int rela_size = rela_tab_get_size(rela_tab);
    
    // The data size
    // TODO: Run pass 1 on the data section here
    
    StrList *data_section = str_list_create();
    str_list_add(data_section, "Hello!");
    
    int data_size = str_list_size(data_section) - 1;
    
    // The code size
    // TODO: Run pass 1 on the code section here
    int code_size = 13;
    
    // Create the sections
    int offset = 64 * 8;
    int name_pos = 0;
    
    SHList *section_list = sh_list_create();
    
    // Create the section string header
    name_pos = str_list_get_pos(shstrlist, ".shstrtab");
    sh_create_strtab(section_list, offset, shstrlist_size, name_pos);
    offset += shstrlist_size;
    
    // Create the string table (which other sections use)
    name_pos = str_list_get_pos(shstrlist, ".strtab");
    sh_create_strtab(section_list, offset, strtab_size, name_pos);
    offset += strtab_size;
    
    // Create the symbol table header
    name_pos = str_list_get_pos(shstrlist, ".symtab");
    sh_create_symtab(section_list, offset, symtab_size, name_pos, symtab_start);
    offset += symtab_size;
    
    // Create the data section
    name_pos = str_list_get_pos(shstrlist, ".data");
    sh_create_data(section_list, offset, data_size, name_pos);
    offset += data_size;
    
    // Create the code (.text) section
    name_pos = str_list_get_pos(shstrlist, ".text");
    sh_create_text(section_list, offset, code_size, name_pos);
    offset += code_size;
    
    // Create the .rela.text section
    name_pos = str_list_get_pos(shstrlist, ".rela.text");
    sh_create_rela_text(section_list, offset, rela_size, name_pos);
    offset += rela_size;
    
    // Write
    elf_write_header(file, section_list);
    
    str_list_write(shstrlist, true, file);
    str_list_write(strtab, true, file);
    
    symtab_write(symtab, file);
    
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
    
    rela_tab_write(rela_tab, file);
    
    // Cleanup
    rela_tab_destroy(rela_tab);
    symtab_destroy(symtab);
    str_list_destroy(strtab);
    str_list_destroy(shstrlist);
    sh_list_destroy(section_list);
    
    fclose(file);

    return 0;
}
