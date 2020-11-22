#include <asm/asm.hpp>
#include <asm/amd64.hpp>

// Writes the REX prefix
void amd64_rex_prefix(bool size64, bool extend_dest, bool extend_src, FILE *file) {
    int mask = 0b01000000;
    
    if (size64) mask |= 0b01001000;
    if (extend_dest) mask |= 0b01000001;
    if (extend_src) mask |= 0b01000100;
    
    fputc(mask, file);
}

// Encodes a single register instruction
void amd64_r1(Reg64 reg, int digit, FILE *file) {
    switch (digit) {
        case 4: {
            switch (reg) {
                case RAX: 
                case R8:  fputc(0xE0, file); break;
                case RCX: 
                case R9:  fputc(0xE1, file); break;
                case RDX: 
                case R10: fputc(0xE2, file); break;
                case RBX: 
                case R11: fputc(0xE3, file); break;
                case RSP: 
                case R12: fputc(0xE4, file); break;
                case RBP: 
                case R13: fputc(0xE5, file); break;
                case RSI: 
                case R14: fputc(0xE6, file); break;
                case RDI: 
                case R15: fputc(0xE7, file); break;
            }
        } break;

        case 5: {
            switch (reg) {
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
        } break;
    }
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

// A utility function for converting a 32-bit register to a 64-bit one
Reg64 amd64_r32_to_r64(Reg32 reg) {
    switch (reg) {
        case EAX: return RAX;
        case R8D: return R8;
        case ECX: return RCX;
        case R9D: return R9;
        case EDX: return RDX;
        case R10D: return R10;
        case EBX: return RBX;
        case R11D: return R11;
        case ESP: return RSP;
        case R12D: return R12;
        case EBP: return RBP;
        case R13D: return R13;
        case ESI: return RSI;
        case R14D: return R14;
        case EDI: return RDI;
        case R15D: return R15;
    }
    
    return RAX;
}

// A utility function for converting a 16-bit register to a 64-bit one
Reg64 amd64_r16_to_r64(Reg16 reg) {
    switch (reg) {
        case AX: return RAX;
        case R8W: return R8;
        case CX: return RCX;
        case R9W: return R9;
        case DX: return RDX;
        case R10W: return R10;
        case BX: return RBX;
        case R11W: return R11;
        case SP: return RSP;
        case R12W: return R12;
        case BP: return RBP;
        case R13W: return R13;
        case SI: return RSI;
        case R14W: return R14;
        case DI: return RDI;
        case R15W: return R15;
    }
    
    return RAX;
}

