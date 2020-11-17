// This file is part of the "as" assembler.
// Copyright (C) 2020 Patrick Flynn
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <elf/elf_bin.hpp>

extern int data_pass(std::string data, std::vector<Elf64_Sym> *es, std::vector<std::string> *st, std::vector<std::string> *dt);
extern int pass1(std::string data, std::vector<Elf64_Sym> *es, std::vector<std::string> *st, std::vector<Elf64_Rela> *rela_tab);
extern void pass2(std::string data, FILE *f);

std::string load_section(const char *path, const char *section) {
    std::ifstream reader(path);
    if (!reader.is_open()) {
        std::cerr << "Unable to open input." << std::endl;
        return "";
    }
    
    std::string contents = "";
    std::string line = "";
    bool add = false;
    
    while (std::getline(reader, line)) {
        int pos = line.find("section");
        if (pos == 0) {
            pos += 8;
            std::string part = line.substr(pos);
            
            if (part == section) {
                add = true;
            } else {
                add = false;
            }
            
            continue;
        }
        
        if (line.length() > 0 && add) {
            contents += line + "\n";
        }
    }
    
    contents += "\n";
    return contents;
}

// Various string utilities
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
    // Load the input file
    std::string code = load_section(in_path, ".text");
    std::string data = load_section(in_path, ".data");

    // Build the section string table
    std::vector<std::string> *shstrtable = new std::vector<std::string>();
    shstrtable->push_back(".shstrtab");
    shstrtable->push_back(".symtab");
    shstrtable->push_back(".strtab");
    shstrtable->push_back(".rela.text");
    shstrtable->push_back(".data");
    shstrtable->push_back(".text");
    
    // Create the symbol tables
    std::vector<std::string> *strtab = new std::vector<std::string>();
    strtab->push_back(in_path);
    
    std::vector<std::string> *data_values = new std::vector<std::string>();
    std::vector<Elf64_Rela> *rela_tab = new std::vector<Elf64_Rela>();
    
    std::vector<Elf64_Sym> *symtab = new std::vector<Elf64_Sym>();
    elf_generate_symtab(symtab);
    
    // Pass 1
    data_pass(data, symtab, strtab, data_values);
    int code_size = pass1(code, symtab, strtab, rela_tab);
    
    int rela_size = rela_tab->size();
    int start_pos = elf_symtab_sort(symtab);
    
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
    int data_size = get_table_length(data_values) - 1;
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

