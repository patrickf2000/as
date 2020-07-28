#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
    
    char *out_path = "a.out";
    
    for (int i = 1; i<argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i+1 >= argc) {
                puts("Error: No output file specified.");
                return 1;
            }
            
            out_path = strdup(argv[i+1]);
            ++i;
        }
    }

    FILE *file = fopen(out_path, "w");
    
    // Pass 1
    SymbolTable *sym_table = sym_table_init_default();
    int start = parse(argv[1], file, 1, sym_table);
    
    elf_write_header(file, start);
    elf_write_pheader(file, start);
    
    // Pass 2
    parse(argv[1], file, 0, sym_table);
    
    fclose(file);
    
    //Set permissions
    chmod(out_path, 0777);
	
	return 0;
}
