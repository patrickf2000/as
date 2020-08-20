#pragma once

#include <stdio.h>

#include <parser/parser.h>

void build_obj(FILE *file, DataInfo *data, int code_size);
void build_simple_exe(FILE *file);
