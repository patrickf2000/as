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

