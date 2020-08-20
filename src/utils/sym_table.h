#pragma once

typedef struct
{
    int *locations;
    int size;
} SymbolTable;

SymbolTable *sym_table_init(int size);
SymbolTable *sym_table_init_default();
int sym_table_hash(const char *str, int size);
int sym_table_add(SymbolTable *table, const char *str, int value);
int sym_table_get(SymbolTable *table, const char *str);
