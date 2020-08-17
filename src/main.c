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
    int build_elf = 1;
    
    for (int i = 1; i<argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i+1 >= argc) {
                puts("Error: No output file specified.");
                return 1;
            }
            
            out_path = strdup(argv[i+1]);
            ++i;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i+1 > argc) {
                puts("Error: No output type specified.");
                return 1;
            }
            
            if (strcmp(argv[i+1], "bin") == 0) {
                build_elf = 0;
            } else if (strcmp(argv[i+1], "elf") == 0) {
                build_elf = 1;
            } else {
                puts("Error: Invalid output type specified.");
                return 1;
            }
            
            ++i;
        }
    }

    FILE *file = fopen(out_path, "w");
    
    // Pass 1
    SymbolTable *sym_table = sym_table_init_default();
    int start = parse(argv[1], file, 1, sym_table);
    
    if (build_elf) {
        elf_write_header(file, start);
        elf_write_null_header(file, start);
        elf_write_shstrtab(file, start);
        elf_write_symtable(file, start);
        elf_write_strtab(file, start);
        elf_write_text(file, start);
        //elf_write_pheader(file, start);
        
        elf_write_shstrtable(file);
        elf_write_symbols(file);
        elf_write_strtable(file);
    }
    
    // Pass 2
    parse(argv[1], file, 0, sym_table);
    
    //buffer
    for (int i = 0; i<15; i++)
        fputc(0, file);
    
    fclose(file);
    
    //Set permissions
    if (build_elf) chmod(out_path, 0777);
	
	return 0;
}
