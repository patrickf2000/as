#include <stdio.h>
#include <stdint.h>

enum Register
{
    AH, CH, DH, BH,
    
    AL, CL, DL, BL, SIL, DIL, SPL, BPL,
    R8B, R9B, R10B, R11B, R12B, R13B, R14B, R15B,
    
    AX, CX, DX, BX, SP, BP, SI, DI,
    R8W, R9W, R10W, R11W, R12W, R13W, R14W, R15W,
    
    EAX, ECX, EDX, EBX, ESI, EDI, ESP, EBP,
    R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D,
    
    RAX, RCX, RDX, RBX, RSI, RDI, RSP, RBP,
    R8, R9, R10, R11, R12, R13, R14, R15
};

struct Memory
{
    int dsp;
    int base;
    int index;
    int scale;
};

uint8_t encode_modrm(uint8_t mod, uint8_t op1, uint8_t op2)
{
    uint8_t encoding = 0;
    
    op1 += op2 * 8;
    encoding = op1 + (64 * mod);
    
    return encoding;
}

int main()
{
    uint8_t mod = 1;
    uint8_t op1 = 5;
    uint8_t op2 = 3;
    
    uint8_t modrm = encode_modrm(mod, op1, op2);
    printf("\nResult: %x\n", modrm);
    
    return 0;
}
