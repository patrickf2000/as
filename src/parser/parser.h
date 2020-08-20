#pragma once

#include <elf/elf_bin.h>

typedef struct
{
    char *names;
    char *values;
} DataInfo;

void split_file(const char *in_path);
DataInfo *generate_data(const char *in_path);
