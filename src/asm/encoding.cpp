#include <asm/asm.hpp>
#include <asm/amd64.hpp>

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

