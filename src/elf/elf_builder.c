// Copyright 2020 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this 
//	list of conditions and the following disclaimer in the documentation and/or 
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may 
//	be used to endorse or promote products derived from this software 
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <string.h>
#include <stdlib.h>

#include <elf/elf_bin.h>
#include <utils/str_table.h>
#include <utils/sym_table.h>

extern void set_extern_symtab(SymbolTable *table);
extern char *data_val_parse(const char *path, char *data_values);
extern char *data_name_parse(const char *path, char *data_name, SymbolTable *st);
extern char *symbol_parse(const char *path, char *strtab, Elf64_SymTab *table);
extern int parse(const char *path, FILE *f, PassType pt, SymbolTable *st, Elf64_RelaTab *rt);
extern int parse_start_pos();

// Builds a relocatable object file
void build_obj(FILE *file, const char *in_path)
{
    // Build the section string table
    char *shstrtable = calloc(1, sizeof(char));
    int shstrtab_name = str_table_add(".shstrtab", shstrtable);
    int symtab_name = str_table_add(".symtab", shstrtable);
    int strtab_name = str_table_add(".strtab", shstrtable);
    int rela_text_name = str_table_add(".rela.text", shstrtable);
    int data_name = str_table_add(".data", shstrtable);
    int text_name = str_table_add(".text", shstrtable);
    
    // Setup the initial string table
    int length = strlen("_start") + strlen(in_path) + 3;
    int start_end = strlen("_start") + 1;
    
    char *strtab = calloc(length,sizeof(char));
    strtab[0] = '|';
    strcat(strtab, "_start");
    strtab[start_end] = '|';
    strcat(strtab, in_path);
    
    // Create the symbol tables
    SymbolTable *sym_table = sym_table_init_default();
    SymbolTable *extern_table = sym_table_init_default();
    Elf64_SymTab *symtab = elf_generate_symtab();
    
    Elf64_RelaTab *rela_tab = malloc(sizeof(Elf64_RelaTab));
    rela_tab->size = 0;
    
    set_extern_symtab(extern_table);
    
    // Load the data
    char *data_values = calloc(1, sizeof(char)); 
    data_values = data_val_parse(in_path, data_values);
    
    char *data_names = calloc(1, sizeof(char)); 
    data_names = data_name_parse(in_path, data_names, sym_table);
    
    strtab = elf_insert_data_symbols(symtab, sym_table, data_names, data_values, strtab);
    
    // Pass 1
    strtab = symbol_parse(in_path, strtab, symtab);
    int code_size = parse(in_path, file, Build1, sym_table, rela_tab);
    int rela_size = rela_tab->size;
    int start_pos = elf_symtab_sort(symtab);
    
    // Build the rest
    int offset = 8 * 64;
        
    // The headers
    elf_write_header(file);
    elf_write_null_header(file);
        
    offset = elf_header_shstrtab(file, shstrtab_name, offset, shstrtable);
    offset = elf_header_symtab(file, symtab_name, offset, symtab->size, start_pos);
    offset = elf_header_strtab(file, strtab_name, offset, strtab);
    offset = elf_header_sec_data(file, data_name, offset, data_values);
    offset = elf_header_text(file, text_name, offset, code_size);
    offset = elf_header_rela_text(file, rela_text_name, offset, rela_size);
    
    // Write the first several sections    
    str_table_write(file, shstrtable);
    elf_write_symtab(file, symtab);
    str_table_write(file, strtab);
    elf_write_sec_data(file, data_values);
    
    // Write the code
    parse(in_path, file, Build2, sym_table, rela_tab);
    
    // Write the rela.text section
    elf_write_rela_text(file, rela_tab);
    
    free(shstrtable);
    free(strtab);
    free(rela_tab);
}
