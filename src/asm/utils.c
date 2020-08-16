#include "asm.h"

// Writes the 64-bit prefix
void amd64_64prefix(int size64, int dest64, int src64, FILE *file)
{
    int mask = 0b01001101;
    
    if (!size64) mask &= 0b01000101;
    if (!src64) mask &= 0b01001001;
    if (!dest64) mask &= 0b01001100;
    
    fputc(mask, file);
}

// Encode a register -> register instruction
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
        case R10: reg2 = 0b11011111; break;
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