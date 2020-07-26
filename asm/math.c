#include "asm.h"

// Add two registers
// Format 01 (11 <src> <dest>)
void amd64_add_rr32(Reg32 dest, Reg32 src, FILE *file)
{
    // Write the instruction
    fputc(0x01, file);
    
    // Encode registers
    int mask = 0b11111111;
    int reg1, reg2;
    
    // The destination
    switch (dest)
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
    switch (src)
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
