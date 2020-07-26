#include "asm.h"

// Move integer immediate to 32-bit register
// Format: <op> <imm>
void amd64_mov_reg32_imm(Reg32 reg, int imm, FILE *file)
{
    // Write the instruction
    // The instruction is different per register
    switch (reg)
    {
        case RAX: fputc(0xB8, file); break;
        case RCX: fputc(0xB9, file); break;
        case RDX: fputc(0xBA, file); break;
        case RBX: fputc(0xBB, file); break;
        case RSP: fputc(0xBC, file); break;
        case RBP: fputc(0xBD, file); break;
        case RSI: fputc(0xBE, file); break;
        case RDI: fputc(0xBF, file); break;
    }
    
    // Write the immediate value
    fwrite(&imm, sizeof(int), 1, file);
}

// Move one register to another
void amd64_mov_rr(Reg64 r1, Reg64 r2, FILE *file)
{
    //First write the instruction
    fputc(0x48, file);    // The 64-bit prefix
    fputc(0x89, file);    // The instruction
    
    //Now decode the registers
    // Binary format: 11 <source> <dest>
    int mask = 0b11111111;
    int reg1, reg2;
    
    // The destination
    switch (r1)
    {
        case RAX: reg1 = 0b11111000; break;
        case RCX: reg1 = 0b11111001; break;
        case RDX: reg1 = 0b11111011; break;
        case RBX: reg1 = 0b11111011; break;
        case RSP: reg1 = 0b11111100; break;
        case RBP: reg1 = 0b11111101; break;
        case RSI: reg1 = 0b11111110; break;
        case RDI: reg1 = 0b11111111; break;
    }
    
    // The source
    switch (r2)
    {
        case RAX: reg2 = 0b11000111; break;
        case RCX: reg2 = 0b11001111; break;
        case RDX: reg2 = 0b11011111; break;
        case RBX: reg2 = 0b11011111; break;
        case RSP: reg2 = 0b11100111; break;
        case RBP: reg2 = 0b11101111; break;
        case RSI: reg2 = 0b11110111; break;
        case RDI: reg2 = 0b11111111; break;
    }
    
    // Do the math and write
    mask = mask & reg1;
    mask = mask & reg2;
    fputc(mask, file);
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

// Encodes registers that have either a source/destination effective address
// with a displacement, and a source/destination 32-bit register
void amd64_mov_dsp16(Reg64 mem, Reg32 r, int dsp, FILE *file)
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

// Move a 32-bit register to memory location
// Format: 89 
void amd64_mov_m_reg32(Reg64 dest, int dsp, Reg32 src, FILE *file)
{
    // Write the instruction
    fputc(0x89, file);
    
    // The registers
    amd64_mov_dsp16(dest, src, dsp, file);
}

// Move memory location to a register
// Format: 8B <registers> <displacement>
void amd64_mov_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file)
{
    // Write the instruction
    fputc(0x8B, file);
    
    // Write the registers
    amd64_mov_dsp16(src, dest, dsp, file);
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

