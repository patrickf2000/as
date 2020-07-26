#include "asm.h"

// Compare 32-bit register and immediate
// Foramt: 83 <reg> <imm>
void amd_cmp_reg32_imm(Reg32 op1, int op2, FILE *file)
{
    // The instruction
    fputc(0x83, file);
    
    // The registers
    switch (op1)
    {
        case RAX: fputc(0xF8, file); break;
        case RCX: fputc(0xF9, file); break;
        case RDX: fputc(0xFA, file); break;
        case RBX: fputc(0xFB, file); break;
        case RSP: fputc(0xFC, file); break;
        case RBP: fputc(0xFD, file); break;
        case RSI: fputc(0xFE, file); break;
        case RDI: fputc(0xFF, file); break;
    }

    // Write the immediate
    fputc(op2, file);
}

// The relative jump instructions
// Format: <instruction> <loco>
void amd64_jmp(Jmp jtype, int loco, FILE *file)
{
    // Determine jump type
    switch (jtype)
    {
        case JMP: fputc(0xEB, file); break;
        case JE: fputc(0x74, file); break;
        case JNE: fputc(0x75, file); break;
        case JG: fputc(0x7F, file); break;
        case JL: fputc(0x7C, file); break;
        case JGE: fputc(0x7D, file); break;
        case JLE: fputc(0x7E, file); break;
    }

    if (loco < 0) loco = 256 + loco;
    fputc(loco, file);
}

// Write the syscall instruction
// 0F 05
void amd64_syscall(FILE *file)
{
    fputc(0x0F, file);
    fputc(0x05, file);
}
