#include <stdio.h>
#include <stdlib.h>

#include <sym_table.h>
#include <elf/elf_bin.h>

extern int parse(const char *path, FILE *f, int pass1, SymbolTable *st);

int main(int argc, char *argv[]) 
{
    if (argc == 1) 
    {
        puts("Error: No input");
        return 1;
    }

    FILE *file = fopen("out.bin", "w");
    
    // Pass 1
    SymbolTable *sym_table = sym_table_init_default();
    int start = parse(argv[1], file, 1, sym_table);
    
    elf_write_header(file, start);
    elf_write_pheader(file, start);
    
    // Pass 2
    parse(argv[1], file, 0, sym_table);
    
    fclose(file);
	
	return 0;
}
