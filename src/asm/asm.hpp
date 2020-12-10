// This file is part of the "as" assembler.
// Copyright (C) 2020 Patrick Flynn
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <stdio.h>

enum Reg64 {
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

enum Reg32 {
    NONE,
    
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

enum Reg16 {
    AX,
    BX,
    CX,
    DX,
    SP,
    BP,
    SI,
    DI,
    
    R8W,
    R9W,
    R10W,
    R11W,
    R12W,
    R13W,
    R14W,
    R15W
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

void amd64_jmp(Jmp jtype, int loco, FILE *file);
void amd64_call(int loco, FILE *file);
void amd64_ret(FILE *file);

void amd64_push_reg64(Reg64 reg, FILE *file);

void amd64_mov_m_int(Reg64 dest, int dsp, int imm, FILE *file);
void amd64_mov_m_reg32(Reg64 dest, int dsp, Reg32 src, FILE *file);
void amd64_mov_m_reg64(Reg64 dest, int dsp, Reg64 src, FILE *file);
void amd64_mov_r8_mrr(Reg16H dest, Reg64 src1, Reg64 src2, FILE *file);
void amd64_mov_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file);
void amd64_mov_reg64_mem(Reg64 dest, Reg64 src, int dsp, FILE *file);

void amd64_xor_rr32(Reg32 dest, Reg32 src, FILE *file);

