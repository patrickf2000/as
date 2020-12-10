#include <asm/amd64.hpp>

// Writes the REX prefix
void amd64_rex_prefix(bool size64, bool extend_dest, bool extend_src, FILE *file, bool sib) {
    int mask = 0b01000000;
    
    if (size64) mask |= 0b01001000;
    if (extend_dest && sib) mask |= 0b01000010;
    if (extend_dest && !sib) mask |= 0b01000001;
    if (extend_src) mask |= 0b01000100;
    
    fputc(mask, file);
}

// Encodes a single register instruction
// Use these in examples like this: 80 /7 imm
void amd64_r1(Reg64 reg, int digit, FILE *file) {
    unsigned char byte = 0xC0;

    switch (reg) {
        case RAX:
        case R8:  byte = 0xC0; break;
        case RCX:
        case R9:  byte = 0xC1; break;
        case RDX:
        case R10: byte = 0xC2; break;
        case RBX:
        case R11: byte = 0xC3; break;
        case RSP:
        case R12: byte = 0xC4; break;
        case RBP:
        case R13: byte = 0xC5; break;
        case RSI:
        case R14: byte = 0xC6; break;
        case RDI:
        case R15: byte = 0xC7; break;
    }

    int size = digit * 8;
    byte += (unsigned char)size;
    fputc(byte, file);
}

void amd64_r1(Reg32 reg, int digit, FILE *file) {
    auto reg64 = amd64_r32_to_r64(reg);
    amd64_r1(reg64, digit, file);
}

// Encode a register -> register instruction 
void amd64_rr(Reg64 r1, Reg64 r2, FILE *file) {
    // Binary format: 11 <source> <dest>
    int mask = 0b11111111;
    int reg1, reg2;
    
    // The destination
    switch (r1) {
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
    switch (r2) {
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

void amd64_rr(Reg32 r1, Reg32 r2, FILE *file) {
    auto r64_1 = amd64_r32_to_r64(r1);
    auto r64_2 = amd64_r32_to_r64(r2);
    amd64_rr(r64_1, r64_2, file);
}

void amd64_rr(Reg16 r1, Reg16 r2, FILE *file) {
    auto r64_1 = amd64_r16_to_r64(r1);
    auto r64_2 = amd64_r16_to_r64(r2);
    amd64_rr(r64_1, r64_2, file);
}

