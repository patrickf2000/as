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

#include "asm.hpp"
#include <asm/amd64.hpp>

void amd64_write_prefix(int size, Reg64 r1, Reg64 r2, FILE *file) {
    bool extend_r1 = r1 > RDI;
    bool extend_r2 = r2 > RDI;
    
    switch (size) {
        case 8:
        case 16: fputc(0x66, file);
        case 32: {
            if (extend_r1 || extend_r2) {
                amd64_rex_prefix(false, extend_r1, extend_r2, file);
            }
        } break;
        
        case 64: {
            amd64_rex_prefix(true, extend_r1, extend_r2, file);
        } break;
    }
}

void amd64_write_prefix(Reg64 r1, Reg64 r2, FILE *file) {
    amd64_write_prefix(64, r1, r2, file);
}

void amd64_write_prefix(Reg32 r1, Reg32 r2, FILE *file) {
    auto r1_64 = amd64_r32_to_r64(r1);
    auto r2_64 = amd64_r32_to_r64(r2);
    amd64_write_prefix(32, r1_64, r2_64, file);
}

void amd64_write_prefix(Reg32 r1, Reg64 r2, FILE *file) {
    auto r1_64 = amd64_r32_to_r64(r1);
    amd64_write_prefix(32, r1_64, r2, file);
}

void amd64_write_prefix(Reg16 r1, Reg16 r2, FILE *file) {
    auto r1_64 = amd64_r16_to_r64(r1);
    auto r2_64 = amd64_r16_to_r64(r2);
    amd64_write_prefix(8, r1_64, r2_64, file);
}

// A utility function for converting a 32-bit register to a 64-bit one
Reg64 amd64_r32_to_r64(Reg32 reg) {
    switch (reg) {
        case EAX: return RAX;
        case R8D: return R8;
        case ECX: return RCX;
        case R9D: return R9;
        case EDX: return RDX;
        case R10D: return R10;
        case EBX: return RBX;
        case R11D: return R11;
        case ESP: return RSP;
        case R12D: return R12;
        case EBP: return RBP;
        case R13D: return R13;
        case ESI: return RSI;
        case R14D: return R14;
        case EDI: return RDI;
        case R15D: return R15;
    }
    
    return RAX;
}

// A utility function for converting a 16-bit register to a 64-bit one
Reg64 amd64_r16_to_r64(Reg16 reg) {
    switch (reg) {
        case AX: return RAX;
        case R8W: return R8;
        case CX: return RCX;
        case R9W: return R9;
        case DX: return RDX;
        case R10W: return R10;
        case BX: return RBX;
        case R11W: return R11;
        case SP: return RSP;
        case R12W: return R12;
        case BP: return RBP;
        case R13W: return R13;
        case SI: return RSI;
        case R14W: return R14;
        case DI: return RDI;
        case R15W: return R15;
    }
    
    return RAX;
}

