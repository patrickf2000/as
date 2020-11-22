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

// Encodes registers that have either a source/destination effective address
// with a displacement, and a source/destination 32-bit register
void amd64_dsp16(Reg64 mem, Reg64 src, int dsp, FILE *file) {
    // Write the registers
    // Binary format: 1 <dest> <src>
    int reg1, reg2;
    
    // The destination
    switch (mem)
    {
        case RAX: reg1 = 0b1111000; break;
        case RCX: reg1 = 0b1111001; break;
        case RDX: reg1 = 0b1111011; break;
        case RBX: reg1 = 0b1111011; break;
        case RSP: reg1 = 0b1111100; break;
        case RBP: reg1 = 0b1111101; break;
        case RSI: reg1 = 0b1111110; break;
        case RDI: reg1 = 0b1111111; break;
    }
    
    // The source
    switch (src)
    {
        case RAX: reg2 = 0b1000111; break;
        case RCX: reg2 = 0b1001111; break;
        case RDX: reg2 = 0b1010111; break;
        case RBX: reg2 = 0b1011111; break;
        case RSP: reg2 = 0b1100111; break;
        case RBP: reg2 = 0b1101111; break;
        case RSI: reg2 = 0b1110111; break;
        case RDI: reg2 = 0b1111111; break;
    }
    
    // Do the math and write
    int mask = reg1 & reg2;
    fputc(mask, file);
    
    // Determine the displacement
    if (dsp < 0) {
        dsp = dsp * -1;
        dsp = 256 - dsp;
    }
    
    fputc(dsp, file);
}

void amd64_dsp16(Reg64 mem, Reg32 src, int dsp, FILE *file) {
    auto src64 = amd64_r32_to_r64(src);
    amd64_dsp16(mem, src64, dsp, file);
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

// Write the syscall instruction
// 0F 05
void amd64_syscall(FILE *file)
{
    fputc(0x0F, file);
    fputc(0x05, file);
}

// Write the leave instruction
// C9
void amd64_leave(FILE *file)
{
    fputc(0xC9, file);
}

