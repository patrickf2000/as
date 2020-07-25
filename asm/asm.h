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
    RDI
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
    EDI
} Reg32;

void amd64_push_reg64(Reg64 reg, FILE *file);

void amd64_mov_reg32_imm(Reg32 reg, int imm, FILE *file);
void amd64_mov_rr(Reg64 r1, Reg64 r2, FILE *file);
void amd64_mov_m_int(Reg64 dest, int dsp, int imm, FILE *file);
void amd64_mov_m_reg32(Reg64 dest, int dsp, Reg32 src, FILE *file);
void amd64_mov_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file);
void amd64_mov_reg64_mem(Reg64 dest, Reg64 src, int dsp, FILE *file);

void amd64_add_rr32(Reg32 dest, Reg32 src, FILE *file);
void amd64_sub_ri(Reg64 r, int imm, FILE *file);

void amd64_syscall(FILE *file);