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

// Add two registers
// Format 01 (11 <src> <dest>)
void amd64_add_rr32(Reg32 dest, Reg32 src, FILE *file)
{
    // Check the prefix
    int dest_sz = dest > EDI;
    int src_sz = src > EDI;
    amd64_64prefix(0, dest_sz, src_sz, file);

    // Write the instruction
    fputc(0x01, file);
    
    // Encode registers
    int mask = 0b11111111;
    int reg1, reg2;
    
    // The destination
    switch (dest)
    {
        case EAX: 
        case R8D: reg1 = 0b11111000; break;
        case ECX: 
        case R10D: reg1 = 0b11111001; break;
        case EDX: 
        case R11D: reg1 = 0b11111011; break;
        case EBX: 
        case R9D: reg1 = 0b11111011; break;
        case ESP: 
        case R12D: reg1 = 0b11111100; break;
        case EBP: 
        case R13D: reg1 = 0b11111101; break;
        case ESI: 
        case R14D: reg1 = 0b11111110; break;
        case EDI: 
        case R15D: reg1 = 0b11111111; break;
    }
    
    // The source
    switch (src)
    {
        case EAX: 
        case R8D: reg2 = 0b11000111; break;
        case ECX: 
        case R10D: reg2 = 0b11001111; break;
        case EDX: 
        case R11D: reg2 = 0b11011111; break;
        case EBX: 
        case R9D: reg2 = 0b11011111; break;
        case ESP: 
        case R12D: reg2 = 0b11100111; break;
        case EBP: 
        case R13D: reg2 = 0b11101111; break;
        case ESI: 
        case R14D: reg2 = 0b11110111; break;
        case EDI: 
        case R15D: reg2 = 0b11111111; break;
    }
    
    // Do the math and write
    mask = mask & reg1;
    mask = mask & reg2;
    fputc(mask, file);
}

// Add a 32-bit register and an immediate
void amd64_add_r32_imm(Reg32 reg, int imm, FILE *file)
{
    // Write the instruction
    fputc(0x83, file);
    
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
    
    // Write the immediate value
    fputc(imm, file);
}

void amd64_sub_ri(Reg64 r, int imm, FILE *file)
{
    // Write the instruction
    fputc(0x48, file);        // 64-bit prefix
    fputc(0x83, file);        // Opcode
    
    // Write the destination register
    int dest = 0b11101000;
    
    switch (r)
    {
        case RAX: dest = 0b11101000; break;
        case RBX: dest = 0b11101011; break;
        case RCX: dest = 0b11101001; break;
        case RDX: dest = 0b11101010; break;
        case RSP: dest = 0b11101100; break;
        case RBP: dest = 0b11101101; break;
        case RSI: dest = 0b11101110; break;
        case RDI: dest = 0b11101111; break;
    }
    
    fputc(dest, file);
    
    // Write the immediate
    fputc(imm, file);
}
