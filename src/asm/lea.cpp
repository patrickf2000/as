
#include <asm/amd64.hpp>

// Load effective address to 64-bit register
void amd64_lea64(Reg64 dest, Reg64 src, int dsp, FILE *file) {
    // Write the prefix
    // TODO: Add checking if the registers are extended
    amd64_rex_prefix(true, false, false, file);
    
    // Write the instruction
    fputc(0x8D, file);
    
    //Write the registers
    amd64_dsp16(src, dest, dsp, file);
}

// Load effective address with a scale to a 64-bit register
void amd64_lea64_scale(Reg64 dest, Reg64 src, int dsp, int scale, FILE *file) {
    bool extend_dest = dest > RDI;
    bool extend_src = src > RDI;
    amd64_rex_prefix(true, extend_src, extend_dest, file, true);

    fputc(0x8D, file);
    fputc(0x34, file);  // TODO: This RBP specific, which is wrong

    amd64_sib(src, dest, dsp, scale, file);
}
