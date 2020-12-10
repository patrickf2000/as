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

#include <asm/amd64.hpp>

// Compare 16-bit register half and immediage
// Format: 80 <reg> <imm>
void amd64_cmp_reg16h_imm(Reg16H op1, int op2, FILE *file) {
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
void amd64_cmp_reg32_imm(Reg32 op1, int op2, FILE *file) {
    if (op1 > EDI)
        amd64_rex_prefix(false, true, false, file);
    
    fputc(0x83, file);
    amd64_r1(op1, 7, file);
    fputc(op2, file);
}

// Compare 64-bit register and immediate
// Format: <prefix> 83 <reg> <imm>
void amd64_cmp_reg64_imm(Reg64 op1, int op2, FILE *file) {
    bool extend_reg = op1 > RDI;
    amd64_rex_prefix(true, extend_reg, false, file);
        
    // The instruction
    fputc(0x83, file);
    amd64_r1(op1, 7, file);
    fputc(op2, file);
}

// Encode 32-bit register and register
void amd64_cmp_rr32(Reg32 dest, Reg32 src, FILE *file) {
    bool extend_dest = dest > EDI;
    bool extend_src = src > EDI;
    if (extend_dest || extend_src)
        amd64_rex_prefix(false, extend_dest, extend_src, file);
        
    fputc(0x3B, file);
    amd64_rr(dest, src, file);
}

// Encode 32-bit register and memory
// Format: <prefix> 3B <reg> <dsp>
void amd64_cmp_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file) {
    bool extend_dest = dest > EDI;
    bool extend_src = src > RDI;
    if (extend_dest || extend_src)
        amd64_rex_prefix(false, extend_dest, extend_src, file);

    fputc(0x3B, file);
    amd64_dsp16(src, dest, dsp, file);
}

