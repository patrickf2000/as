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

#include <inttypes.h>

#include "asm.h"

// Move integer immediate to 32-bit register
// Format: <op> <imm>
void amd64_mov_reg32_imm(Reg32 reg, int imm, FILE *file)
{
    // Check the registers- if its one of the 64-bit ones, we need a prefix
    if (reg > EDI)
        amd64_64prefix(0, 1, 0, file);

    // Write the instruction
    // The instruction is different per register
    switch (reg)
    {
        case EAX: 
        case R8D: fputc(0xB8, file); break;
        
        case ECX:
        case R9D: fputc(0xB9, file); break;
        
        case EDX:
        case R10D: fputc(0xBA, file); break;
        
        case EBX:
        case R11D: fputc(0xBB, file); break;
        
        case ESP:
        case R12D: fputc(0xBC, file); break;
        
        case EBP:
        case R13D: fputc(0xBD, file); break;
        
        case ESI:
        case R14D: fputc(0xBE, file); break;
        
        case EDI: 
        case R15D: fputc(0xBF, file); break;
    }
    
    // Write the immediate value
    fwrite(&imm, sizeof(int), 1, file);
}

// Move integer immediate to 64-bit register
// Format: <prefix> <op> <imm>
void amd64_mov_reg64_imm(Reg64 reg, int imm, int extend, FILE *file)
{
    // Check the registers- if its one of the 64-bit ones, we need a prefix
    if (reg > RDI)
        amd64_64prefix(1, 1, 0, file);
    else if (extend)
        amd64_64prefix(1, 0, 0, file);

    // Write the instruction
    // The instruction is different per register
    switch (reg)
    {
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
    
    // Write the immediate value
    if (extend)
    {
        int64_t imm64 = (int64_t)imm;
        fwrite(&imm64, sizeof(int64_t), 1, file);
    }
    else
    {
        fwrite(&imm, sizeof(int), 1, file);
    }
}

// Move one register to another (32-bit)
void amd64_mov_rr32(Reg32 r1, Reg32 r2, FILE *file)
{
    //Write the prefix
    int dest_sz = r1 > RDI;
    int src_sz = r2 > RDI;
    if (dest_sz || src_sz)
        amd64_64prefix(0, dest_sz, src_sz, file);
    
    //Write the instruction
    fputc(0x89, file);
    
    //Now decode the registers
    amd64_rr(r1, r2, file);
}

// Move one register to another (64-bit)
void amd64_mov_rr64(Reg64 r1, Reg64 r2, FILE *file)
{
    //Write the prefix
    int dest_sz = r1 > RDI;
    int src_sz = r2 > RDI;
    amd64_64prefix(1, dest_sz, src_sz, file);
    
    //Write the instruction
    fputc(0x89, file);
    
    //Now decode the registers
    amd64_rr(r1, r2, file);
}

// Move an integer immediate to a memory location
// Format: C7 <reg> <displacement> <immediate>
void amd64_mov_m_int(Reg64 dest, int dsp, int imm, FILE *file)
{
    // Write the instruction
    fputc(0xC7, file);
    
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
    
    // Write the immediate
    fwrite(&imm, sizeof(int), 1, file);
}

// Move a 32-bit register to memory location
// Format: 89 
void amd64_mov_m_reg32(Reg64 dest, int dsp, Reg32 src, FILE *file)
{
    // Write the instruction
    fputc(0x89, file);
    
    // The registers
    amd64_dsp16(dest, src, dsp, file);
}

// Move a 64-bit register to memory location
// Format: <prefix> 89
void amd64_mov_m_reg64(Reg64 dest, int dsp, Reg64 src, FILE *file)
{
    // Write the prefix
    // TODO: This needs to be properly generated
    fputc(0x48, file);
    
    // Write the instruction
    fputc(0x89, file);
    
    // And the registers
    amd64_dsp16(dest, src, dsp, file);
}

// Move memory location to 8-bit half register using reg+reg indexing
void amd64_mov_r8_mrr(Reg16H dest, Reg64 src1, Reg64 src2, FILE *file)
{
    // Write the instruction
    fputc(0x8A, file);

    // Write the destination
    switch (dest)
    {
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
    switch (src1)
    {
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
    switch (src2)
    {
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
void amd64_mov_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file)
{
    // Write the instruction
    fputc(0x8B, file);
    
    // Write the registers
    amd64_dsp16(src, dest, dsp, file);
}

// Move memory location to 64-bit register
// Its exactly the same as moving to 32-bit, except for the 64-bit prefix
void amd64_mov_reg64_mem(Reg64 dest, Reg64 src, int dsp, FILE *file)
{
    // Write the prefix
    fputc(0x48, file);
    
    // Write the rest
    amd64_mov_reg32_mem(dest, src, dsp, file);
}

