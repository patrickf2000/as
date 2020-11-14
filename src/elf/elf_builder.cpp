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

#include <vector>
#include <string>
#include <map>

#include <elf/elf_bin.hpp>

extern int pass1(std::string data, std::vector<Elf64_Sym> *es, std::vector<std::string> *st);
extern void pass2(std::string data, FILE *f);

int get_str_pos(std::vector<std::string> *strtab, std::string to_find) {
    int pos = 1;
    
    for (int i = 0; i<strtab->size(); i++) {
        auto current = strtab->at(i);
        
        if (current == to_find) {
            break;
        } else {
            pos += current.length() + 1;
        }
    }
    
    return pos;
}

int get_table_length(std::vector<std::string> *table) {
    int length = 1;
    
    for (int i = 0; i<table->size(); i++) {
        length += table->at(i).length() + 1;
    }
    
    return length;
}

// Builds a relocatable object file
void build_obj(FILE *file, const char *in_path)
{
    // Build the section string table
    std::vector<std::string> *shstrtable = new std::vector<std::string>();
    shstrtable->push_back(".shstrtab");
    shstrtable->push_back(".symtab");
    shstrtable->push_back(".strtab");
    shstrtable->push_back(".rela.text");
    shstrtable->push_back(".data");
    shstrtable->push_back(".text");
    
    // Setup the string table
    std::vector<std::string> *strtab = new std::vector<std::string>();
    strtab->push_back(in_path);
    
    // Create the symbol tables
    std::vector<Elf64_Rela> *rela_tab = new std::vector<Elf64_Rela>();
    
    std::vector<Elf64_Sym> *symtab = new std::vector<Elf64_Sym>();
    elf_generate_symtab(symtab);
    
    // Load the data
    std::vector<std::string> *data_values = new std::vector<std::string>();
    
    // Pass 1
    // Test code. This needs to go
    std::string code = "";
        code += ".global _start\n";
        code += "_start:\n";
        code += "mov rax, 60\n";
        code += "mov rdi, 5\n";
        code += "syscall\n";
    int code_size = pass1(code, symtab, strtab);
    
    int rela_size = rela_tab->size();
    //int start_pos = elf_symtab_sort(symtab);
    int start_pos = 4;
    
    // Build the rest
    int offset = 8 * 64;
        
    // The headers
    elf_write_header(file);
    elf_write_null_header(file);
        
    int shstrtab_name = get_str_pos(shstrtable, ".shstrtab");
    int shstrtab_size = get_table_length(shstrtable);
    offset = elf_header_shstrtab(file, shstrtab_name, offset, shstrtab_size);
    
    int symtab_name = get_str_pos(shstrtable, ".symtab");
    int symtab_size = symtab->size() * sizeof(Elf64_Sym);
    offset = elf_header_symtab(file, symtab_name, offset, symtab->size(), start_pos);
    
    int strtab_name = get_str_pos(shstrtable, ".strtab");
    int strtab_size = get_table_length(strtab);
    offset = elf_header_strtab(file, strtab_name, offset, strtab_size);
    
    int data_name = get_str_pos(shstrtable, ".data");
    int data_size = get_table_length(data_values);
    offset = elf_header_sec_data(file, data_name, offset, data_size);
    
    int text_name = get_str_pos(shstrtable, ".text");
    offset = elf_header_text(file, text_name, offset, code_size);
    
    int rela_text_name = get_str_pos(shstrtable, ".rela.text");
    offset = elf_header_rela_text(file, rela_text_name, offset, rela_size);
    
    // Write the first several sections    
    elf_strtab_write(file, shstrtable);
    elf_write_symtab(file, symtab);
    elf_strtab_write(file, strtab);
    elf_write_sec_data(file, data_values);
    
    // Write the code
    pass2(code, file);
    
    // Write the rela.text section
    elf_write_rela_text(file, rela_tab);
}
