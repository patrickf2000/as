
#include <asm/amd64.hpp>

// Encodes the displacement for the division instructions
void amd64_idiv_dsp(Reg64 src, int dsp, FILE *file) {
    switch (src) {
        case RAX:
        case R8:  fputc(0x78, file); break;
        case RCX:
        case R9:  fputc(0x79, file); break;
        case RDX:
        case R10: fputc(0x7A, file); break;
        case RBX:
        case R11: fputc(0x7B, file); break;
        case RSP:
        case R12: fputc(0x7C, file); break;
        case RBP:
        case R13: fputc(0x7D, file); break;
        case RSI:
        case R14: fputc(0x7E, file); break;
        case RDI:
        case R15: fputc(0x7F, file); break;
    }

    // Determine the displacement
    if (dsp < 0) {
        dsp = dsp * -1;
        dsp = 256 - dsp;
    }

    fputc(dsp, file);
}

// Encodes division on 32-bit register
void amd64_idiv_r32(Reg32 src, FILE *file) {
    bool extend_src = src > EDI;
    if (extend_src)
        amd64_rex_prefix(false, extend_src, false, file);

    fputc(0xF7, file);
    amd64_r1(src, 6, file);
}

// Encodes division on 32-bit memory location
void amd64_idiv_mem32(Reg64 src, int dsp, FILE *file) {
    bool extend_src = src > RDI;
    if (extend_src)
        amd64_rex_prefix(false, extend_src, false, file);

    fputc(0xF7, file);
    amd64_idiv_dsp(src, dsp, file);
}
