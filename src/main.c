#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <elf/elf_bin.h>

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

    // Generate the file
    FILE *file = fopen(out_path, "w");
    
    // Build the object
    build_obj(file, argv[1]);
    
    // Clean things up
    fclose(file);
    
    //Set permissions
    chmod(out_path, 0777);
	
	return 0;
}
