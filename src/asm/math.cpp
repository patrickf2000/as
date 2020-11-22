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

// The common functions for math register-register operations
void amd64_math_rr32(Reg32 dest, Reg32 src, unsigned char opcode, FILE *file)
{
    // Check the prefix
    int dest_sz = dest > EDI;
    int src_sz = src > EDI;
    amd64_64prefix(0, dest_sz, src_sz, file);

    // Write the instruction
    fputc(opcode, file);
    
    //Now encode the registers
    amd64_rr(dest, src, file);
}

void amd64_math_rr64(Reg64 dest, Reg64 src, unsigned char opcode, FILE *file)
{
    // Check the prefix
    int dest_sz = dest > RDI;
    int src_sz = src > RDI;
    amd64_64prefix(1, dest_sz, src_sz, file);

    // Write the instruction
    fputc(opcode, file);
    
    //Now encode the registers
    amd64_rr(dest, src, file);
}

// Add two registers
// Format 01 (11 <src> <dest>)
void amd64_add_rr32(Reg32 dest, Reg32 src, FILE *file)
{
    amd64_math_rr32(dest, src, 0x01, file);
}

void amd64_add_rr64(Reg64 dest, Reg64 src, FILE *file)
{
    amd64_math_rr64(dest, src, 0x01, file);
}

// Used by the two add register,immediate instructions
void amd64_add_ri(Reg64 reg, FILE *file)
{
    // The register
    switch (reg)
    {
        case RAX: fputc(0xC0, file); break;
        case RCX: fputc(0xC1, file); break;
        case RDX: fputc(0xC2, file); break;
        case RBX: fputc(0xC3, file); break;
        case RSP: fputc(0xC4, file); break;
        case RBP: fputc(0xC5, file); break;
        case RSI: fputc(0xC6, file); break;
        case RDI: fputc(0xC7, file); break;
    }
}

void amd64_add_ri32(Reg32 reg, FILE *file)
{
    // The register
    switch (reg)
    {
        case EAX: fputc(0xC0, file); break;
        case ECX: fputc(0xC1, file); break;
        case EDX: fputc(0xC2, file); break;
        case EBX: fputc(0xC3, file); break;
        case ESP: fputc(0xC4, file); break;
        case EBP: fputc(0xC5, file); break;
        case ESI: fputc(0xC6, file); break;
        case EDI: fputc(0xC7, file); break;
    }
}

// Add a 32-bit register and an immediate
void amd64_add_r32_imm(Reg32 reg, int imm, FILE *file)
{
    // Write the instruction
    fputc(0x83, file);
    
    // Encode the register
    amd64_add_ri32(reg, file);
    
    // Write the immediate value
    fputc(imm, file);
}

void amd64_add_r64_imm(Reg64 reg, int imm, FILE *file)
{
    // Write the prefix
    if (reg > RDI)
        amd64_64prefix(1, 1, 0, file);
    else
        amd64_64prefix(1, 0, 0, file);
    
    // Write the instruction
    fputc(0x83, file);
    
    // Encode the register
    amd64_add_ri(reg, file);
    
    // Write the immediate value
    fputc(imm, file);
}

// Add a register and memory location
void amd64_add_r32_mem(Reg32 reg, Reg64 src, int dsp, FILE *file)
{
    // Write the instruction
    fputc(0x03, file);
    
    // The registers
    amd64_dsp16(src, reg, dsp, file);
}

// Add a 64-bit register and a memory location
void amd64_add_r64_mem(Reg64 reg, Reg64 src, int dsp, FILE *file)
{
    // Write the prefix
    if (reg > RDI)
        amd64_64prefix(1, 1, 0, file);
    else
        amd64_64prefix(1, 0, 0, file);
        
    // Write the instruction
    fputc(0x03, file);
    
    // The registers
    amd64_dsp16(src, reg, dsp, file);
}

// Add an immediate with a 32-bit memory location
void amd64_add_dw_mem_imm(Reg64 dest, int dsp, int imm, FILE *file)
{
    // Write the instruction
    fputc(0x83, file);
    
    // Encode the destination
    amd64_mem_imm(dest, dsp, file);
    
    // Write the immediate
    fputc(imm, file);
}

// Subtract one register value from another
void amd64_sub_rr32(Reg32 dest, Reg32 src, FILE *file)
{
    amd64_math_rr32(dest, src, 0x29, file);
}

void amd64_sub_rr64(Reg64 dest, Reg64 src, FILE *file)
{
    amd64_math_rr64(dest, src, 0x29, file);
}

// Subtract an immediate from a 32-bit register value
void amd64_sub_r32_imm(Reg32 reg, int imm, FILE *file)
{
    // Write the instruction
    fputc(0x83, file);
    
    // Encode the register
    amd64_r1(reg, 5, file);
    
    // Write the immediate
    fputc(imm, file);
}

// Subtract an immediate from a 64-bit register value
void amd64_sub_r64_imm(Reg64 reg, int imm, FILE *file)
{
    // Write the prefix
    if (reg > RDI)
        amd64_64prefix(1, 1, 0, file);
    else
        amd64_64prefix(1, 0, 0, file);

    // Write the instruction
    fputc(0x83, file);
    
    // Encode the register
    amd64_r1(reg, 5, file);
    
    // Write the immediate
    fputc(imm, file);
}

// Signed-multiply two 32-bit register values
void amd64_imul_rr32(Reg32 dest, Reg32 src, FILE *file)
{
    // Write the prefix if needed
    int dest_sz = dest > EDI;
    int src_sz = src > EDI;
    if (dest_sz || src_sz)
        amd64_64prefix(0, src_sz, dest_sz, file);
        
    // Write the instruction
    fputc(0x0F, file);
    fputc(0xAF, file);
    
    // The registers
    amd64_rr(src, dest, file);
}

// Signed-multiply two 64-bit register values
// Note: For multiplication, we have to reverse the dest and src registers
void amd64_imul_rr64(Reg64 dest, Reg64 src, FILE *file)
{
    // Write the prefix
    int dest_sz = dest > RDI;
    int src_sz = src > RDI;
    amd64_64prefix(1, src_sz, dest_sz, file);
        
    // Write the instruction
    fputc(0x0F, file);
    fputc(0xAF, file);
    
    // The registers
    amd64_rr(src, dest, file);
}

// Signed multiply register with eax value
void amd64_imul_r32(Reg32 reg, FILE *file)
{
    // Write the prefix if needed
    if (reg > EDI) amd64_64prefix(0, 1, 0, file);
    
    // Write the instruction
    fputc(0xF7, file);
    
    // Encode the registers
    amd64_r1(reg, 5, file);
}

// Signed multiply register with rax value
void amd64_imul_r64(Reg64 reg, FILE *file)
{
    // Write the prefix
    int reg_sz = reg > RDI;
    amd64_64prefix(1, reg_sz, 0, file);
    
    // Write the instruction
    fputc(0xF7, file);
    
    // Encode the registers
    amd64_r1(reg, 5, file);
}

// Signed multiply 32-bit register and immediate
void amd64_imul_r32_imm(Reg32 dest, Reg32 src, int imm, FILE *file)
{
    // Write the prefix
    int dest_sz = dest > EDI;
    int src_sz = src > EDI;
    if (dest_sz || src_sz) amd64_64prefix(0, src_sz, dest_sz, file);
    
    // Write the instruction
    fputc(0x69, file);
    
    // Encode the registers
    amd64_rr(src, dest, file);
    
    // Write the immediate
    fwrite(&imm, sizeof(int), 1, file);
}

// Signed multiply 64-bit register and immediates
void amd64_imul_r64_imm(Reg64 dest, Reg64 src, int imm, FILE *file)
{
    // Write the prefix
    int dest_sz = dest > RDI;
    int src_sz = src > RDI;
    amd64_64prefix(1, src_sz, dest_sz, file);
    
    // Write the instruction
    fputc(0x69, file);
    
    // Encode the registers
    amd64_rr(src, dest, file);
    
    // Write the immediate
    fwrite(&imm, sizeof(int), 1, file);
}

// Add a register and memory location
void amd64_imul_r32_mem(Reg32 reg, Reg64 src, int dsp, FILE *file) {
    fputc(0x0F, file);
    fputc(0xAF, file);
    amd64_dsp16(src, reg, dsp, file);
}
