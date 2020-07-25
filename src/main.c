#include <stdio.h>
#include <stdlib.h>

#include <elf_bin.h>

extern void parse(const char *path, FILE *f);

int main(int argc, char *argv[]) 
{
    FILE *file = fopen("out.bin", "w");
    
    // Write the ELF stuff
    elf_write_header(file);
    elf_write_pheader(file);

    // Parse and write
	if (argc > 1) {
		parse(argv[1], file);
	} else {
		parse("", file);
	}
    
    fclose(file);
	
	return 0;
}
