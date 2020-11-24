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

#include <inttypes.h>

#include "asm.hpp"
#include <asm/amd64.hpp>

// Writes the opcode for the move-immediate instructions
void amd64_mov_imm(Reg64 reg, FILE *file) {
    // Write the instruction
    // The instruction is different per register
    switch (reg) {
        case RAX: 
        case R8: fputc(0xB8, file); break;
        
        case RCX:
        case R9: fputc(0xB9, file); break;
        
        case RDX:
        case R10: fputc(0xBA, file); break;
        
        case RBX:
        case R11: fputc(0xBB, file); break;
        
        case RSP:
        case R12: fputc(0xBC, file); break;
        
        case RBP:
        case R13: fputc(0xBD, file); break;
        
        case RSI:
        case R14: fputc(0xBE, file); break;
        
        case RDI: 
        case R15: fputc(0xBF, file); break;
    }
}

// Move integer immediate to 16-bit register
// Format: 66 <rex> <op> <imm>
void amd64_mov_r16_imm(Reg16 reg, int imm, FILE *file) {
    fputc(0x66, file);
    
    if (reg > DI)
        amd64_rex_prefix(false, true, false, file);
        
    auto reg64 = amd64_r16_to_r64(reg);
    amd64_mov_imm(reg64, file);
    
    short imm2 = (short)imm;
    fwrite(&imm2, sizeof(short), 1, file);
}

// Move integer immediate to 32-bit register
// Format: <op> <imm>
void amd64_mov_r32_imm(Reg32 reg, int imm, FILE *file) {
    if (reg > EDI)
        amd64_rex_prefix(false, true, false, file);
    
    auto reg64 = amd64_r32_to_r64(reg);
    amd64_mov_imm(reg64, file);
    
    fwrite(&imm, sizeof(int), 1, file);
}

// Move integer immediate to 64-bit register
// Format: <prefix> <op> <imm>
void amd64_mov_r64_imm(Reg64 reg, int imm, FILE *file) {
    bool reg_size = (reg > RDI);
    amd64_rex_prefix(true, reg_size, false, file);

    amd64_mov_imm(reg, file);
    
    int64_t imm64 = (int64_t)imm;
    fwrite(&imm64, sizeof(int64_t), 1, file);
}

// Move one register to another (16-bit)
// Syntax: 66 <rex> 89 <reg>
void amd64_mov_rr16(Reg16 r1, Reg16 r2, FILE *file) {
    fputc(0x66, file);
    
    bool dest_extend = r1 > DI;
    bool src_extend = r2 > DI;
    if (dest_extend || src_extend)
        amd64_rex_prefix(false, dest_extend, src_extend, file);
        
    fputc(0x89, file);
    amd64_rr(r1, r2, file);
}

// Move one register to another (32-bit)
void amd64_mov_rr32(Reg32 r1, Reg32 r2, FILE *file) {
    bool dest_extend = r1 > EDI;
    bool src_extend = r2 > EDI;
    if (dest_extend || src_extend)
        amd64_rex_prefix(false, dest_extend, src_extend, file);
    
    fputc(0x89, file);
    amd64_rr(r1, r2, file);
}

// Move one register to another (64-bit)
void amd64_mov_rr64(Reg64 r1, Reg64 r2, FILE *file) {
    //Write the prefix
    bool dest_extend = r1 > RDI;
    bool src_extend = r2 > RDI;
    amd64_rex_prefix(true, dest_extend, src_extend, file);
    
    fputc(0x89, file);
    amd64_rr(r1, r2, file);
}

// Move an integer immediate to a memory location
// Format: C7 <reg> <displacement> <immediate>
void amd64_mov_m_int(Reg64 dest, int dsp, int imm, FILE *file) {
    // Write the instruction
    fputc(0xC7, file);
    
    // Encode the destination
    amd64_mem_imm(dest, dsp, file);
    
    // Write the immediate
    fwrite(&imm, sizeof(int), 1, file);
}

void amd64_mov_m64_imm(Reg64 dest, int dsp, int imm, FILE *file) {
    bool dest_extend = dest > RDI;
    amd64_rex_prefix(true, dest_extend, false, file);

    fputc(0xC7, file);      //Opcode
    amd64_mem_imm(dest, dsp, file);

    fwrite(&imm, sizeof(int), 1, file);
}

// Move a 32-bit register to memory location
// Format: 89 
void amd64_mov_m_reg32(Reg64 dest, int dsp, Reg32 src, FILE *file) {
    // Write the instruction
    fputc(0x89, file);
    
    // The registers
    amd64_dsp16(dest, src, dsp, file);
}

// Move a 64-bit register to memory location
// Format: <prefix> 89
void amd64_mov_m_reg64(Reg64 dest, int dsp, Reg64 src, FILE *file) {
    // Write the prefix
    // TODO: This needs to be properly generated
    fputc(0x48, file);
    
    // Write the instruction
    fputc(0x89, file);
    
    // And the registers
    amd64_dsp16(dest, src, dsp, file);
}

// Move memory location to 8-bit half register using reg+reg indexing
void amd64_mov_r8_mrr(Reg16H dest, Reg64 src1, Reg64 src2, FILE *file) {
    // Write the instruction
    fputc(0x8A, file);

    // Write the destination
    switch (dest) {
        case AL: fputc(0x04, file); break;
        case CL: fputc(0x0C, file); break;
        case DL: fputc(0x14, file); break;
        case BL: fputc(0x1C, file); break;
        case AH: fputc(0x24, file); break;
        case CH: fputc(0x2C, file); break;
        case DH: fputc(0x34, file); break;
        case BH: fputc(0x3C, file); break;
    }
    
    // The registers
    unsigned char reg1 = 0;
    unsigned char reg2 = 0;
    
    // Index
    switch (src1) {
        case RAX: reg1 = 0b00111000; break;
        case RCX: reg1 = 0b00111001; break;
        case RDX: reg1 = 0b00111011; break;
        case RBX: reg1 = 0b00111011; break;
        case RSP: reg1 = 0b00111100; break;
        case RBP: reg1 = 0b00111101; break;
        case RSI: reg1 = 0b00111110; break;
        case RDI: reg1 = 0b00111111; break;
    }
    
    // Base
    switch (src2) {
        case RAX: reg2 = 0b00000111; break;
        case RCX: reg2 = 0b00001111; break;
        case RDX: reg2 = 0b00010111; break;
        case RBX: reg2 = 0b00011111; break;
        case RSP: reg2 = 0b00100111; break;
        case RBP: reg2 = 0b00101111; break;
        case RSI: reg2 = 0b00110111; break;
        case RDI: reg2 = 0b00111111; break;
    }
    
    // Write
    unsigned char base = reg1 & reg2;
    fputc(base, file);
}

// Move memory location to a register
// Format: 8B <registers> <displacement>
void amd64_mov_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file) {
    // Write the instruction
    fputc(0x8B, file);
    
    // Write the registers
    amd64_dsp16(src, dest, dsp, file);
}

// Move memory location to 64-bit register
// Its exactly the same as moving to 32-bit, except for the 64-bit prefix
// TODO: Most of the body to this needs to go elsewhere
void amd64_mov_reg64_mem(Reg64 dest, Reg64 src, int dsp, FILE *file) {
    // Write the prefix
    bool extend_dest = dest > RDI;
    amd64_rex_prefix(true, false, extend_dest, file);

    // Write the rest
    fputc(0x8B, file);
    amd64_dsp16(src, dest, dsp, file);
}

// Load effective address to 64-bit register
void amd64_lea64(Reg64 dest, Reg64 src, int dsp, FILE *file) {
    // Write the prefix
    // TODO: Add checking if the registers are extended
    amd64_64prefix(1, 0, 0, file);
    
    // Write the instruction
    fputc(0x8D, file);
    
    //Write the registers
    amd64_dsp16(src, dest, dsp, file);
}

