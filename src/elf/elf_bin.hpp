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

#pragma once

#include <stdio.h>
#include <elf.h>

#include <vector>
#include <string>

// In the functions that write the header, we take two parameters:
// offset -> The current location based on headers and amount of data
// data -> So we can determine size
//
// The header functions return the new location.

// The types
typedef struct
{
    Elf64_Sym *symbols;
    int size;
} Elf64_SymTab;

typedef struct
{
    Elf64_Rela *symbols;
    int size;
} Elf64_RelaTab;

typedef enum
{
	DataVal,
	DataName,
	SymParse,
	Build1,
	Build2
} PassType;

// Build functions
int get_str_pos(std::vector<std::string> *strtab, std::string to_find);
int get_table_length(std::vector<std::string> *table);
void build_obj(FILE *file, const char *in_path);

// The functions
void elf_write_header(FILE *file);
void elf_write_null_header(FILE *file);

int elf_header_shstrtab(FILE *file, int name_pos, int offset, int size);
int elf_header_strtab(FILE *file, int name_pos, int offset, int size);
void elf_strtab_write(FILE *file, std::vector<std::string> *table);

int elf_header_symtab(FILE *file, int name_pos, int offset, int count, int start_pos);
void elf_write_symtab(FILE *file, std::vector<Elf64_Sym> *symtab);
void elf_generate_symtab(std::vector<Elf64_Sym> *symtab);
int elf_add_symbol(std::vector<Elf64_Sym> *table, int name_pos, int value, int is_data, int type);
int elf_symtab_sort(std::vector<Elf64_Sym> *table);
//char *elf_insert_data_symbols(Elf64_SymTab *symtab, SymbolTable *dataPos, char *names, char *values, char *strtab);

int elf_header_rela_text(FILE *file, int name_pos, int offset, int size);
void elf_rela_add(std::vector<Elf64_Rela> *table, int code_offset, int data_offset);
void elf_rela_add_func(std::vector<Elf64_Rela> *table, int code_offset, int symtab_pos);
void elf_write_rela_text(FILE *file, std::vector<Elf64_Rela> *table);
int elf_header_sec_data(FILE *file, int name_pos, int offset, int size);
void elf_write_sec_data(FILE *file, std::vector<std::string> *data);

int elf_header_text(FILE *file, int name_pos, int offset, int size);

