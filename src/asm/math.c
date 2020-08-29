// Copyright 2020 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this 
//	list of conditions and the following disclaimer in the documentation and/or 
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may 
//	be used to endorse or promote products derived from this software 
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "asm.h"

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

// Add a 32-bit register and an immediate
void amd64_add_r32_imm(Reg32 reg, int imm, FILE *file)
{
    // Write the instruction
    fputc(0x83, file);
    
    // Encode the register
    amd64_add_ri(reg, file);
    
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

// Subtract an immediate value from register contents
void amd64_sub_ri(Reg64 reg, int imm, FILE *file)
{
    // Encode the register
    switch (reg)
    {
        case RAX: 
        case R8:  fputc(0xE8, file); break;
        case RCX: 
        case R9:  fputc(0xE9, file); break;
        case RDX: 
        case R10: fputc(0xEA, file); break;
        case RBX: 
        case R11: fputc(0xEB, file); break;
        case RSP: 
        case R12: fputc(0xEC, file); break;
        case RBP: 
        case R13: fputc(0xED, file); break;
        case RSI: 
        case R14: fputc(0xEE, file); break;
        case RDI: 
        case R15: fputc(0xEF, file); break;
    }
    
    // Write the immediate
    fputc(imm, file);
}

// Subtract an immediate from a 32-bit register value
void amd64_sub_r32_imm(Reg32 reg, int imm, FILE *file)
{
    // Write the instruction
    fputc(0x83, file);
    
    amd64_sub_ri(reg, imm, file);
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
    
    amd64_sub_ri(reg, imm, file);
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
