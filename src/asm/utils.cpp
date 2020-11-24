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
#include <asm/amd64.hpp>

// Writes the 64-bit prefix
void amd64_64prefix(int size64, int dest64, int src64, FILE *file)
{
    int mask = 0b01001101;
    
    if (!size64) mask &= 0b01000101;
    if (!src64) mask &= 0b01001001;
    if (!dest64) mask &= 0b01001100;
    
    fputc(mask, file);
}

// Used for instructions that perform operations on immediates to memory
void amd64_mem_imm(Reg64 dest, int dsp, FILE *file)
{
    // Determine the register
    switch (dest)
    {
        case RAX: fputc(0x40, file); break;
        case RCX: fputc(0x41, file); break;
        case RDX: fputc(0x42, file); break;
        case RBX: fputc(0x43, file); break;
        case RSP: break; //TODO: Error
        case RBP: fputc(0x45, file); break;
        case RSI: fputc(0x46, file); break;
        case RDI: fputc(0x47, file); break;
    }
    
    // Determine the displacement
    if (dsp < 0) {
        dsp = dsp * -1;
        dsp = 256 - dsp;
    }
    
    fputc(dsp, file);
}
