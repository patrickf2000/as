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

// Compare 32-bit register and immediate
// Format: 83 <reg> <imm>
void amd64_cmp_reg32_imm(Reg32 op1, int op2, FILE *file)
{
    // The instruction
    fputc(0x83, file);
    
    // The registers
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

// The relative jump instructions
// Format: <instruction> <loco>
void amd64_jmp(Jmp jtype, int loco, FILE *file)
{
    // Determine jump type
    switch (jtype)
    {
        case JMP: fputc(0xEB, file); break;
        case JE: fputc(0x74, file); break;
        case JNE: fputc(0x75, file); break;
        case JG: fputc(0x7F, file); break;
        case JL: fputc(0x7C, file); break;
        case JGE: fputc(0x7D, file); break;
        case JLE: fputc(0x7E, file); break;
    }

    if (loco < 0) loco = 256 + loco;
    fputc(loco, file);
}

// The relative jump call
// Format: E8 <loco>
// With jumps, the backwards displacement is 4,294,967,296
void amd64_call(int loco, FILE *file)
{
    fputc(0xE8, file);
    
    if (loco < 0) loco = 4294967296 + loco;
    fwrite(&loco, sizeof(int), 1, file);
}

// The return instruction
void amd64_ret(FILE *file)
{
    fputc(0xC3, file);
}

