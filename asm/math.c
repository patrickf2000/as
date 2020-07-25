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
