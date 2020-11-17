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

// Compare 16-bit register half and immediage
// Format: 80 <reg> <imm>
void amd64_cmp_reg16h_imm(Reg16H op1, int op2, FILE *file)
{
    // The instruction
    fputc(0x80, file);
    
    // The registers
    switch (op1)
    {
        case AL: fputc(0xF8, file); break;
        case CL: fputc(0xF9, file); break;
        case DL: fputc(0xFA, file); break;
        case BL: fputc(0xFB, file); break;
        case AH: fputc(0xFC, file); break;
        case CH: fputc(0xFD, file); break;
        case DH: fputc(0xFE, file); break;
        case BH: fputc(0xFF, file); break;
    }

    // Write the immediate
    fputc(op2, file);
}

// Encode registers for the register-immediate instructions

// Compare 32-bit register and immediate
// Format: 83 <reg> <imm>
void amd64_cmp_reg32_imm(Reg32 op1, int op2, FILE *file)
{
    // The instruction
    fputc(0x83, file);
    
    // The registers
    switch (op1)
    {
        case EAX: fputc(0xF8, file); break;
        case ECX: fputc(0xF9, file); break;
        case EDX: fputc(0xFA, file); break;
        case EBX: fputc(0xFB, file); break;
        case ESP: fputc(0xFC, file); break;
        case EBP: fputc(0xFD, file); break;
        case ESI: fputc(0xFE, file); break;
        case EDI: fputc(0xFF, file); break;
    }

    // Write the immediate
    fputc(op2, file);
}

// Compare 64-bit register and immediate
// Format: <prefix> 83 <reg> <imm>
void amd64_cmp_reg64_imm(Reg64 op1, int op2, FILE *file)
{
    // Write the prefix
    if (op1 > RDI)
        amd64_64prefix(1, 1, 0, file);
    else
        amd64_64prefix(1, 0, 0, file);
        
    // The instruction
    fputc(0x83, file);
        
    switch (op1)
    {
        case RAX: fputc(0xF8, file); break;
        case RCX: fputc(0xF9, file); break;
        case RDX: fputc(0xFA, file); break;
        case RBX: fputc(0xFB, file); break;
        case RSP: fputc(0xFC, file); break;
        case RBP: fputc(0xFD, file); break;
        case RSI: fputc(0xFE, file); break;
        case RDI: fputc(0xFF, file); break;
    }
    
    // Write the immediate
    fputc(op2, file);
}
