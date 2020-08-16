#include "asm.h"

// 32-bit XOR with two registers
// 31 <encoding>
void amd64_xor_rr32(Reg32 dest, Reg32 src, FILE *file)
{
    // Write the instruction
    fputc(0x31, file);
    
    // Encode the registers
    amd64_rr(dest, src, file);
}