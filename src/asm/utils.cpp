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

#include "asm.hpp"

// Writes the 64-bit prefix
void amd64_64prefix(int size64, int dest64, int src64, FILE *file)
{
    int mask = 0b01001101;
    
    if (!size64) mask &= 0b01000101;
    if (!src64) mask &= 0b01001001;
    if (!dest64) mask &= 0b01001100;
    
    fputc(mask, file);
}

// Encode a register -> register instruction (64-bit version)
void amd64_rr(Reg64 r1, Reg64 r2, FILE *file)
{
    // Binary format: 11 <source> <dest>
    int mask = 0b11111111;
    int reg1, reg2;
    
    // The destination
    switch (r1)
    {
        case RAX: 
        case R8: reg1 = 0b11111000; break;
        case RCX: 
        case R9: reg1 = 0b11111001; break;
        case RDX: 
        case R10: reg1 = 0b11111010; break;
        case RBX:
        case R11: reg1 = 0b11111011; break;
        case RSP: 
        case R12: reg1 = 0b11111100; break;
        case RBP: 
        case R13: reg1 = 0b11111101; break;
        case RSI: 
        case R14: reg1 = 0b11111110; break;
        case RDI: 
        case R15: reg1 = 0b11111111; break;
    }
    
    // The source
    switch (r2)
    {
        case RAX: 
        case R8: reg2 = 0b11000111; break;
        case RCX: 
        case R9: reg2 = 0b11001111; break;
        case RDX: 
        case R10: reg2 = 0b11010111; break;
        case RBX: 
        case R11: reg2 = 0b11011111; break;
        case RSP: 
        case R12: reg2 = 0b11100111; break;
        case RBP: 
        case R13: reg2 = 0b11101111; break;
        case RSI: 
        case R14: reg2 = 0b11110111; break;
        case RDI: 
        case R15: reg2 = 0b11111111; break;
    }
    
    // Do the math and write
    mask = mask & reg1;
    mask = mask & reg2;
    fputc(mask, file);
}

void amd64_rr32(Reg32 r1, Reg32 r2, FILE *file)
{
    // Binary format: 11 <source> <dest>
    int mask = 0b11111111;
    int reg1, reg2;
    
    // The destination
    switch (r1)
    {
        case EAX: 
        case R8D: reg1 = 0b11111000; break;
        case ECX: 
        case R9D: reg1 = 0b11111001; break;
        case EDX: 
        case R10D: reg1 = 0b11111010; break;
        case EBX:
        case R11D: reg1 = 0b11111011; break;
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
    switch (r2)
    {
        case EAX: 
        case R8D: reg2 = 0b11000111; break;
        case ECX: 
        case R9D: reg2 = 0b11001111; break;
        case EDX: 
        case R10D: reg2 = 0b11010111; break;
        case EBX: 
        case R11D: reg2 = 0b11011111; break;
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

// Encodes a single register instruction
void amd64_r1(Reg64 reg, FILE *file)
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
}

void amd64_r1_32(Reg32 reg, FILE *file)
{
    Reg64 rnew;
    switch (reg)
    {
        case EAX: rnew = RAX; break;
        case ECX: rnew = RCX; break;
        case EDX: rnew = RDX; break;
        case EBX: rnew = RBX; break;
        case ESP: rnew = RSP; break;
        case EBP: rnew = RBP; break;
        case ESI: rnew = RSI; break;
        case EDI: rnew = RDI; break;
    }
    
    amd64_r1(rnew, file);
}

// Encodes registers that have either a source/destination effective address
// with a displacement, and a source/destination 32-bit register
void amd64_dsp16(Reg64 mem, Reg32 r, int dsp, FILE *file)
{
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
    switch (r)
    {
        case EAX: reg2 = 0b1000111; break;
        case ECX: reg2 = 0b1001111; break;
        case EDX: reg2 = 0b1010111; break;
        case EBX: reg2 = 0b1011111; break;
        case ESP: reg2 = 0b1100111; break;
        case EBP: reg2 = 0b1101111; break;
        case ESI: reg2 = 0b1110111; break;
        case EDI: reg2 = 0b1111111; break;
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

void amd64_dsp16_64(Reg64 mem, Reg64 r, int dsp, FILE *file)
{
    Reg32 rnew;
    switch (r)
    {
        case RAX: rnew = EAX; break;
        case RCX: rnew = ECX; break;
        case RDX: rnew = EDX; break;
        case RBX: rnew = EBX; break;
        case RSP: rnew = ESP; break;
        case RBP: rnew = EBP; break;
        case RSI: rnew = ESI; break;
        case RDI: rnew = EDI; break;
    }
    
    amd64_dsp16(mem, rnew, dsp, file);
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

