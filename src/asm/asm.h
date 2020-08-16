#pragma once

#include <stdio.h>

typedef enum
{
    RAX,
    RBX,
    RCX,
    RDX,
    RSP,
    RBP,
    RSI,
    RDI,
    
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15
} Reg64;

typedef enum
{
    EAX,
    EBX,
    ECX,
    EDX,
    ESP,
    EBP,
    ESI,
    EDI,
    
    R8D,
    R9D,
    R10D,
    R11D,
    R12D,
    R13D,
    R14D,
    R15D
} Reg32;

typedef enum
{
    AL,
    BL,
    CL,
    DL,
    
    AH,
    BH,
    CH,
    DH
} Reg16H;

typedef enum
{
    JMP,
    JE,
    JNE,
    JG,
    JL,
    JGE,
    JLE
} Jmp;

void amd64_write_string(const char *str, FILE *file);

void amd64_cmp_reg16h_imm(Reg16H op1, int op2, FILE *file);
void amd64_cmp_reg32_imm(Reg32 op1, int op2, FILE *file);
void amd64_jmp(Jmp jtype, int loco, FILE *file);
void amd64_call(int loco, FILE *file);
void amd64_ret(FILE *file);

void amd64_push_reg64(Reg64 reg, FILE *file);

void amd64_mov_reg32_imm(Reg32 reg, int imm, FILE *file);
void amd64_mov_reg64_imm(Reg64 reg, int imm, int extend, FILE *file);
void amd64_mov_rr32(Reg32 r1, Reg32 r2, FILE *file);
void amd64_mov_rr64(Reg64 r1, Reg64 r2, FILE *file);
void amd64_mov_m_int(Reg64 dest, int dsp, int imm, FILE *file);
void amd64_mov_m_reg32(Reg64 dest, int dsp, Reg32 src, FILE *file);
void amd64_mov_m_reg64(Reg64 dest, int dsp, Reg64 src, FILE *file);
void amd64_mov_r8_mrr(Reg16H dest, Reg64 src1, Reg64 src2, FILE *file);
void amd64_mov_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file);
void amd64_mov_reg64_mem(Reg64 dest, Reg64 src, int dsp, FILE *file);

void amd64_xor_rr32(Reg32 dest, Reg32 src, FILE *file);

void amd64_add_rr32(Reg32 dest, Reg32 src, FILE *file);
void amd64_add_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_sub_ri(Reg64 r, int imm, FILE *file);

void amd64_64prefix(int size64, int dest64, int src64, FILE *file);
void amd64_rr(Reg64 r1, Reg64 r2, FILE *file);
void amd64_syscall(FILE *file);
void amd64_leave(FILE *file);