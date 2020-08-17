#pragma once

void elf_write_header(FILE *file, int start);
void elf_write_pheader(FILE *file, int start);
void elf_write_null_header(FILE *file, int start);

void elf_write_shstrtab(FILE *file, int start);
void elf_write_shstrtable(FILE *file);

void elf_write_symtable(FILE *file, int start);
void elf_write_symbols(FILE *file);

void elf_write_strtab(FILE *file, int start);
void elf_write_strtable(FILE *file);

void elf_write_text(FILE *file, int start);
