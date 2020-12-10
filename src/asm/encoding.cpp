#include <asm/asm.hpp>
#include <asm/amd64.hpp>

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

// Used for instructions that perform operations on immediates to memory
void amd64_mem_imm(Reg64 dest, int dsp, FILE *file)
{
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
}

