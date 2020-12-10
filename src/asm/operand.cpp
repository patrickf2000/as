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

// An internal function to help calculate the mod-rm byte
void amd64_mod_rm(Reg64 reg, int digit, unsigned char start, FILE *file) {
    switch (reg) {
        case RAX:
        case R8: break;
        case RCX:
        case R9:  start += 0x01; break;
        case RDX:
        case R10: start += 0x02; break;
        case RBX:
        case R11: start += 0x03; break;
        case RSP:
        case R12: start += 0x04; break;
        case RBP:
        case R13: start += 0x05; break;
        case RSI:
        case R14: start += 0x06; break;
        case RDI:
        case R15: start += 0x07; break;
    }

    int size = digit * 8;
    start += (unsigned char)size;
    fputc(start, file);
}

// Encodes a single register instruction
// Use these in examples like this: 80 /7 imm
void amd64_r1(Reg64 reg, int digit, FILE *file) {
    amd64_mod_rm(reg, digit, 0xC0, file);
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

// Encodes an 8-bit displacement
void amd64_dsp8(Reg64 src, int dsp, int digit, FILE *file) {
    amd64_mod_rm(src, digit, 0x40, file);
    
    // Determine the displacement
    if (dsp < 0) {
        dsp = dsp * -1;
        dsp = 256 - dsp;
    }

    fputc(dsp, file);
}

void amd64_rr_dsp8(Reg64 dest, Reg64 src, int dsp, FILE *file) {
    int digit = 0;
    
    switch (dest) {
        case RAX:
        case R8: break;
        case RCX:
        case R9:  digit = 1; break;
        case RDX:
        case R10: digit = 2; break;
        case RBX:
        case R11: digit = 3; break;
        case RSP:
        case R12: digit = 4; break;
        case RBP:
        case R13: digit = 5; break;
        case RSI:
        case R14: digit = 6; break;
        case RDI:
        case R15: digit = 7; break;
    }
    
    amd64_dsp8(src, dsp, digit, file);
}

void amd64_rr_dsp8(Reg32 dest, Reg64 src, int dsp, FILE *file) {
    auto dest64 = amd64_r32_to_r64(dest);
    amd64_rr_dsp8(dest64, src, dsp, file);
}

