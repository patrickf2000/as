// Copyright 2020 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this 
//	list of conditions and the following disclaimer in the documentation and/or 
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may 
//	be used to endorse or promote products derived from this software 
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <stdio.h>

enum Reg64
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
};

enum Reg32
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
};

enum Reg16H
{
    AL,
    BL,
    CL,
    DL,
    
    AH,
    BH,
    CH,
    DH
};

enum Jmp
{
    JMP,
    JE,
    JNE,
    JG,
    JL,
    JGE,
    JLE
};

void amd64_cmp_reg16h_imm(Reg16H op1, int op2, FILE *file);
void amd64_cmp_reg32_imm(Reg32 op1, int op2, FILE *file);
void amd64_cmp_reg64_imm(Reg64 op1, int op2, FILE *file);

void amd64_write_string(const char *str, FILE *file);

void amd64_jmp(Jmp jtype, int loco, FILE *file);
void amd64_call(int loco, FILE *file);
void amd64_ret(FILE *file);

void amd64_push_reg64(Reg64 reg, FILE *file);

void amd64_mov_reg32_imm(Reg32 reg, int imm, FILE *file);
void amd64_mov_reg64_imm(Reg64 reg, int imm, FILE *file);
void amd64_mov_rr32(Reg32 r1, Reg32 r2, FILE *file);
void amd64_mov_rr64(Reg64 r1, Reg64 r2, FILE *file);
void amd64_mov_m_int(Reg64 dest, int dsp, int imm, FILE *file);
void amd64_mov_m_reg32(Reg64 dest, int dsp, Reg32 src, FILE *file);
void amd64_mov_m_reg64(Reg64 dest, int dsp, Reg64 src, FILE *file);
void amd64_mov_r8_mrr(Reg16H dest, Reg64 src1, Reg64 src2, FILE *file);
void amd64_mov_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file);
void amd64_mov_reg64_mem(Reg64 dest, Reg64 src, int dsp, FILE *file);
void amd64_lea64(Reg64 dest, Reg64 src, int dsp, FILE *file);

void amd64_xor_rr32(Reg32 dest, Reg32 src, FILE *file);

void amd64_add_rr32(Reg32 dest, Reg32 src, FILE *file);
void amd64_add_rr64(Reg64 dest, Reg64 src, FILE *file);
void amd64_add_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_add_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_add_r32_mem(Reg32 reg, Reg64 src, int dsp, FILE *file);
void amd64_add_r64_mem(Reg64 reg, Reg64 src, int dsp, FILE *file);
void amd64_add_dw_mem_imm(Reg64 dest, int dsp, int imm, FILE *file);
void amd64_sub_rr32(Reg32 dest, Reg32 src, FILE *file);
void amd64_sub_rr64(Reg64 dest, Reg64 src, FILE *file);
void amd64_sub_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_sub_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_imul_rr32(Reg32 dest, Reg32 src, FILE *file);
void amd64_imul_rr64(Reg64 dest, Reg64 src, FILE *file);
void amd64_imul_r32(Reg32 reg, FILE *file);
void amd64_imul_r64(Reg64 reg, FILE *file);
void amd64_imul_r32_imm(Reg32 dest, Reg32 src, int imm, FILE *file);
void amd64_imul_r64_imm(Reg64 dest, Reg64 src, int imm, FILE *file);

void amd64_64prefix(int size64, int dest64, int src64, FILE *file);
void amd64_rr(Reg64 r1, Reg64 r2, FILE *file);
void amd64_rr32(Reg32 r1, Reg32 r2, FILE *file);
void amd64_r1(Reg64 reg, FILE *file);
void amd64_r1_32(Reg32 reg, FILE *file);
void amd64_dsp16(Reg64 mem, Reg32 r, int dsp, FILE *file);
void amd64_dsp16_64(Reg64 mem, Reg64 r, int dsp, FILE *file);
void amd64_mem_imm(Reg64 dest, int dsp, FILE *file);
void amd64_syscall(FILE *file);

void amd64_leave(FILE *file);