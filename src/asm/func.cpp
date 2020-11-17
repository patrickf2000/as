// This file is part of the "as" assembler.
// Copyright (C) 2020 Patrick Flynn
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "asm.hpp"

// The relative jump instructions
// Format: <instruction> <loco>
void amd64_jmp(Jmp jtype, int loco, FILE *file)
{
    // Determine jump type
    switch (jtype)
    {
        case JMP: fputc(0xEB, file); break;
        case JE: fputc(0x74, file); break;
        case JNE: fputc(0x75, file); break;
        case JG: fputc(0x7F, file); break;
        case JL: fputc(0x7C, file); break;
        case JGE: fputc(0x7D, file); break;
        case JLE: fputc(0x7E, file); break;
    }

    if (loco < 0) loco = 256 + loco;
    fputc(loco, file);
}

// The relative jump call
// Format: E8 <loco>
// With jumps, the backwards displacement is 4,294,967,296 (2 ^ 32)
void amd64_call(int loco, FILE *file)
{
    fputc(0xE8, file);
    
    if (loco < 0) loco = 4294967296 + loco;
    fwrite(&loco, sizeof(int), 1, file);
}

// The return instruction
void amd64_ret(FILE *file)
{
    fputc(0xC3, file);
}
