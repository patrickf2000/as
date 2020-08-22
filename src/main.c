#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <utils/sym_table.h>
#include <parser/parser.h>
#include <elf/elf_builder.h>

//extern int parse(const char *path, FILE *f, int pass1, SymbolTable *st);

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
    
    // Split the file into multiple sections
    split_file(argv[1]);
    //DataInfo *data = generate_data("data.asm");

    // Generate the file
    FILE *file = fopen(out_path, "w");
    
    // Build the ELF
    if (build_elf) {
        build_obj(file);
    }
    
    // Clean things up
    fclose(file);
    //free(data);
    
    //Set permissions
    if (build_elf) chmod(out_path, 0777);
	
	return 0;
}
