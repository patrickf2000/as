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

#include <asm/amd64.hpp>

// Signed-multiply two 32-bit register values
void amd64_imul_rr32(Reg32 dest, Reg32 src, FILE *file) {
    bool extend_dest = dest > EDI;
    bool extend_src = src > EDI;
    if (extend_dest || extend_src)
        amd64_rex_prefix(false, extend_src, extend_dest, file);

    fputc(0x0F, file);          // Opcode
    fputc(0xAF, file);
    amd64_rr(src, dest, file);
}

// Signed-multiply two 64-bit register values
// Note: For multiplication, we have to reverse the dest and src registers
void amd64_imul_rr64(Reg64 dest, Reg64 src, FILE *file) {
    bool extend_dest = dest > RDI;
    bool extend_src = src > RDI;
    amd64_rex_prefix(true, extend_src, extend_dest, file);

    fputc(0x0F, file);          // Opcode
    fputc(0xAF, file);
    amd64_rr(src, dest, file);
}

// Signed multiply register with eax value
void amd64_imul_r32(Reg32 reg, FILE *file) {
    if (reg > EDI)
        amd64_rex_prefix(false, true, false, file);

    fputc(0xF7, file);          // Opcode
    amd64_r1(reg, 5, file);
}

// Signed multiply register with rax value
void amd64_imul_r64(Reg64 reg, FILE *file) {
    bool extend_reg = reg > RDI;
    amd64_rex_prefix(true, extend_reg, false, file);

    fputc(0xF7, file);          // Opcode
    amd64_r1(reg, 5, file);
}

// Signed multiply 32-bit register and immediate
void amd64_imul_r32_imm(Reg32 dest, Reg32 src, int imm, FILE *file) {
    bool extend_dest = dest > EDI;
    bool extend_src = src > EDI;
    if (extend_dest || extend_src)
        amd64_rex_prefix(false, extend_src, extend_dest, file);

    fputc(0x69, file);                  // Opcode
    amd64_rr(src, dest, file);
    fwrite(&imm, sizeof(int), 1, file);
}

// Signed multiply 64-bit register and immediates
void amd64_imul_r64_imm(Reg64 dest, Reg64 src, int imm, FILE *file) {
    bool extend_dest = dest > RDI;
    bool extend_src = src > RDI;
    amd64_rex_prefix(true, extend_src, extend_dest, file);

    fputc(0x69, file);                  // Opcode
    amd64_rr(src, dest, file);
    fwrite(&imm, sizeof(int), 1, file);
}

// Add a register and memory location
void amd64_imul_r32_mem(Reg32 reg, Reg64 src, int dsp, FILE *file) {
    fputc(0x0F, file);      // Opcode
    fputc(0xAF, file);
    amd64_rr_dsp8(reg, src, dsp, file);
}

