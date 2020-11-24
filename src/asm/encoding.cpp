#include <asm/asm.hpp>
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
void amd64_r1(Reg64 reg, int digit, FILE *file) {
    switch (digit) {
        case 0: {
            switch (reg) {
                case RAX:
                case R8:  fputc(0xC0, file); break;
                case RCX:
                case R9:  fputc(0xC1, file); break;
                case RDX:
                case R10: fputc(0xC2, file); break;
                case RBX:
                case R11: fputc(0xC3, file); break;
                case RSP:
                case R12: fputc(0xC4, file); break;
                case RBP:
                case R13: fputc(0xC5, file); break;
                case RSI:
                case R14: fputc(0xC6, file); break;
                case RDI:
                case R15: fputc(0xC7, file); break;
            }
        } break;

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

// Encodes effective addressing of a register with 0-displacment
void amd64_dsp0(Reg64 mem, FILE *file) {
    switch (mem) {
        case RAX:
        case R8:  fputc(0x00, file); break;
        case RCX:
        case R9:  fputc(0x01, file); break;
        case RDX:
        case R10: fputc(0x02, file); break;
        case RBX:
        case R11: fputc(0x03, file); break;
        case RSP:
        case R12: fputc(0x04, file); break;
        case RBP:
        case R13: fputc(0x05, file); break;
        case RSI:
        case R14: fputc(0x06, file); break;
        case RDI:
        case R15: fputc(0x07, file); break;
    }
}

void amd64_dsp0(Reg64 mem, Reg64 dest, FILE *file) {
    unsigned char byte = 0x00;

    switch (mem) {
        case RAX:
        case R8:  byte = 0x00; break;
        case RCX:
        case R9:  byte = 0x01; break;
        case RDX:
        case R10: byte = 0x02; break;
        case RBX:
        case R11: byte = 0x03; break;
        case RSP:
        case R12: byte = 0x04; break;
        case RBP:
        case R13: byte = 0x05; break;
        case RSI:
        case R14: byte = 0x06; break;
        case RDI:
        case R15: byte = 0x07; break;
    }

    switch (dest) {
        case RAX:
        case R8:  byte += 0x00; break;
        case RCX:
        case R9:  byte += 0x08; break;
        case RDX:
        case R10: byte += 0x10; break;
        case RBX:
        case R11: byte += 0x18; break;
        case RSP:
        case R12: byte += 0x20; break;
        case RBP:
        case R13: byte += 0x28; break;
        case RSI:
        case R14: byte += 0x30; break;
        case RDI:
        case R15: byte += 0x38; break;
    }

    fputc(byte, file);
}

void amd64_dsp0(Reg64 mem, Reg32 dest, FILE *file) {
    Reg64 dest64 = amd64_r32_to_r64(dest);
    amd64_dsp0(mem, dest64, file);
}

// Encodes registers that have either a source/destination effective address
// with a displacement, and a source/destination 32-bit register
void amd64_dsp16(Reg64 mem, Reg64 src, int dsp, FILE *file) {
    switch (mem) {
        case RBP: {
            switch (src) {
                case RAX:
                case R8:  fputc(0x45, file); break;
                case RCX:
                case R9:  fputc(0x4D, file); break;
                case RDX:
                case R10: fputc(0x55, file); break;
                case RBX:
                case R11: fputc(0x5D, file); break;
                case RSP:
                case R12: fputc(0x65, file); break;
                case RBP:
                case R13: fputc(0x6D, file); break;
                case RSI:
                case R14: fputc(0x75, file); break;
                case RDI:
                case R15: fputc(0x7D, file); break;
            }
        } break;

        case RDI:
        case R15: {
            switch (src) {
                case RAX:
                case R8:  fputc(0x47, file); break;
                case RCX:
                case R9:  fputc(0x4F, file); break;
                case RDX:
                case R10: fputc(0x57, file); break;
                case RBX:
                case R11: fputc(0x5F, file); break;
                case RSP:
                case R12: fputc(0x67, file); break;
                case RBP:
                case R13: fputc(0x6F, file); break;
                case RSI:
                case R14: fputc(0x77, file); break;
                case RDI:
                case R15: fputc(0x7F, file); break;
            }
        } break;

        default: {} //TODO: Add other register combinations
    }

    // Determine the displacement
    if (dsp < 0) {
        dsp = dsp * -1;
        dsp = 256 - dsp;
    }

    fputc(dsp, file);
}

void amd64_dsp16(Reg64 mem, Reg32 src, int dsp, FILE *file) {
    auto src64 = amd64_r32_to_r64(src);
    amd64_dsp16(mem, src64, dsp, file);
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

// Encodes the SIB byte
void amd64_sib(Reg64 mem, Reg64 src, int dsp, int scale, FILE *file) {
    if (scale != 0 && scale != 2 && scale != 4 && scale != 8) {
        //TODO: Error
        return;
    }

    unsigned char byte = 0x00;

    switch (src) {
        case RSI:
        case R14: {
            switch (mem) {
                case RAX:
                case R8: byte = 0x05; break;
                case RCX:
                case R9: byte = 0x0D; break;
                case RDX:
                case R10: byte = 0x15; break;
                case RBX:
                case R11: byte = 0x1D; break;
                case RSP:
                case R12: byte = 0x25; break;
                case RBP:
                case R13: byte = 0x2D; break;
                case RSI:
                case R14: byte = 0x35; break;
                case RDI:
                case R15: byte = 0x3D; break;
            }
        } break;

        // TODO: Add the rest
    }

    switch (scale) {
        case 2: byte += 0x40; break;
        case 4: byte += 0x80; break;
        case 8: byte += 0xC0; break;
    }

    fputc(byte, file);

    // Determine the displacement
    if (dsp < 0) {
        dsp = dsp * -1;
        dsp = 256 - dsp;
    }

    fwrite(&dsp, sizeof(int), 1, file);
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

