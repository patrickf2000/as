#include "asm.h"

void amd64_push_reg64(Reg64 reg, FILE *file)
{
    switch (reg)
    {
        case RAX: fputc(0x50, file); break;
        case RCX: fputc(0x51, file); break;
        case RDX: fputc(0x52, file); break;
        case RBX: fputc(0x53, file); break;
        case RSP: fputc(0x54, file); break;
        case RBP: fputc(0x55, file); break;
        case RSI: fputc(0x56, file); break;
        case RDI: fputc(0x57, file); break;
    }
}