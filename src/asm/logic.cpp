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

#include <asm/asm.hpp>
#include <asm/amd64.hpp>

// 32-bit AND with register and immediate
void amd64_and_r32_imm(Reg32 reg, int imm, FILE *file) {
    if (reg > EDI)
        amd64_rex_prefix(false, true, false, file);

    fputc(0x83, file);
    amd64_r1(reg, 4, file);
    fputc(imm, file);
}

// 64-bit AND with register and immediate
void amd64_and_r64_imm(Reg64 reg, int imm, FILE *file) {
    amd64_write_prefix(reg, RAX, file);
    
    fputc(0x83, file);
    amd64_r1(reg, 4, file);
    fputc(imm, file);
}

// 32-bit OR with register and immediate
void amd64_or_r32_imm(Reg32 reg, int imm, FILE *file) {
    if (reg > EDI)
        amd64_rex_prefix(false, true, false, file);

    fputc(0x81, file);
    amd64_r1(reg, 1, file);
    fwrite(&imm, sizeof(int), 1, file);
}

// 64-bit OR with register and immediate
void amd64_or_r64_imm(Reg64 reg, int imm, FILE *file) {
    amd64_write_prefix(reg, RAX, file);
    
    fputc(0x83, file);
    amd64_r1(reg, 1, file);
    fputc(imm, file);
}

// 32-bit XOR with register and immediate
void amd64_xor_r32_imm(Reg32 reg, int imm, FILE *file) {
    if (reg > EDI)
        amd64_rex_prefix(false, true, false, file);

    fputc(0x81, file);
    amd64_r1(reg, 6, file);
    fwrite(&imm, sizeof(int), 1, file);
}

// 64-bit XOR with register and immediate
void amd64_xor_r64_imm(Reg64 reg, int imm, FILE *file) {
    amd64_write_prefix(reg, RAX, file);
    
    fputc(0x83, file);
    amd64_r1(reg, 6, file);
    fputc(imm, file);
}

// 32-bit XOR with two registers
// 31 <encoding>
// TODO: Fix register encoding
void amd64_xor_rr32(Reg32 dest, Reg32 src, FILE *file) {
    /*bool extend_dest = dest > EDI;
    bool extend_src = src > EDI;
    amd64_rex_prefix(false, extend_dest, extend_src, file);
    */
    fputc(0x31, file);
    amd64_rr(dest, src, file);
}

// 64-bit XOR with two registers
void amd64_xor_rr64(Reg64 dest, Reg64 src, FILE *file) {
    bool extend_dest = dest > RDI;
    bool extend_src = src > RDI;
    amd64_rex_prefix(true, extend_dest, extend_src, file);

    fputc(0x31, file);
    amd64_rr(dest, src, file);
}

// 32-bit shift left with a register and immediate
void amd64_shl_r32_imm(Reg32 reg, int imm, FILE *file) {
    if (reg > EDI)
        amd64_rex_prefix(false, true, false, file);

    fputc(0xC1, file);
    amd64_r1(reg, 4, file);
    fputc(imm, file);
}

// 64-bit shift left with a register and immediate
void amd64_shl_r64_imm(Reg64 reg, int imm, FILE *file) {
    amd64_write_prefix(reg, RAX, file);
    
    fputc(0xC1, file);
    amd64_r1(reg, 4, file);
    fputc(imm, file);
}

// 32-bit shift right with a register and immediate
void amd64_shr_r32_imm(Reg32 reg, int imm, FILE *file) {
    if (reg > EDI)
        amd64_rex_prefix(false, true, false, file);

    fputc(0xC1, file);
    amd64_r1(reg, 5, file);
    fputc(imm, file);
}

// 64-bit shift right with a register and immediate
void amd64_shr_r64_imm(Reg64 reg, int imm, FILE *file) {
    amd64_write_prefix(reg, RAX, file);
    
    fputc(0xC1, file);
    amd64_r1(reg, 5, file);
    fputc(imm, file);
}

