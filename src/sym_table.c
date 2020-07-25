#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sym_table.h>

SymbolTable *sym_table_init(int size)
{
    SymbolTable *table = malloc(sizeof(SymbolTable));
    table->locations = malloc(sizeof(int)*size);
    table->size = size;
    return table;
}

SymbolTable *sym_table_init_default()
{
    return sym_table_init(101);
}

int sym_table_hash(const char *str, int size)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    int result = hash % size;
    return result;
}

int sym_table_add(SymbolTable *table, const char *str, int value)
{
    int location = sym_table_hash(str, table->size);
    //if (!table->locations[location]) return 0;
    table->locations[location] = value;
    //return 1;
}

int sym_table_get(SymbolTable *table, const char *str)
{
    int location = sym_table_hash(str, table->size);
    return table->locations[location];
}
