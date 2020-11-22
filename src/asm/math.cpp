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

// The common functions for math register-register operations
void amd64_math_rr32(Reg32 dest, Reg32 src, unsigned char opcode, FILE *file) {
    bool extend_dest = dest > EDI;
    bool extend_src = src > EDI;
    if (extend_dest || extend_src)
        amd64_rex_prefix(false, extend_dest, extend_src, file);

    fputc(opcode, file);            // Opcode
    amd64_rr(dest, src, file);
}

void amd64_math_rr64(Reg64 dest, Reg64 src, unsigned char opcode, FILE *file) {
    bool extend_dest = dest > RDI;
    bool extend_src = src > RDI;
    amd64_rex_prefix(true, extend_dest, extend_src, file);

    fputc(opcode, file);            // Opcode
    amd64_rr(dest, src, file);
}

// Add two registers
// Format 01 (11 <src> <dest>)
void amd64_add_rr32(Reg32 dest, Reg32 src, FILE *file) {
    amd64_math_rr32(dest, src, 0x01, file);
}

void amd64_add_rr64(Reg64 dest, Reg64 src, FILE *file) {
    amd64_math_rr64(dest, src, 0x01, file);
}

// Add a 32-bit register and an immediate
// Format: 83 /0
void amd64_add_r32_imm(Reg32 reg, int imm, FILE *file) {
    fputc(0x83, file);          // The opcode
    amd64_r1(reg, 0, file);
    fputc(imm, file);
}

// Format: <rex> 83 /0
void amd64_add_r64_imm(Reg64 reg, int imm, FILE *file) {
    bool extend_reg = reg > RDI;
    amd64_rex_prefix(true, extend_reg, false, file);

    fputc(0x83, file);          // The opcode
    amd64_r1(reg, 0, file);
    fputc(imm, file);
}

// Add a register and memory location
void amd64_add_r32_mem(Reg32 reg, Reg64 src, int dsp, FILE *file) {
    fputc(0x03, file);
    amd64_dsp16(src, reg, dsp, file);
}

// Add a 64-bit register and a memory location
void amd64_add_r64_mem(Reg64 reg, Reg64 src, int dsp, FILE *file) {
    bool extend_reg = reg > RDI;
    amd64_rex_prefix(true, extend_reg, false, file);

    fputc(0x03, file);                      // The opcode
    amd64_dsp16(src, reg, dsp, file);
}

// Add an immediate with a 32-bit memory location
void amd64_add_dw_mem_imm(Reg64 dest, int dsp, int imm, FILE *file) {
    fputc(0x83, file);                  // The opcode
    amd64_mem_imm(dest, dsp, file);
    fputc(imm, file);
}

// Subtract one register value from another
void amd64_sub_rr32(Reg32 dest, Reg32 src, FILE *file) {
    amd64_math_rr32(dest, src, 0x29, file);
}

void amd64_sub_rr64(Reg64 dest, Reg64 src, FILE *file) {
    amd64_math_rr64(dest, src, 0x29, file);
}

// Subtract an immediate from a 32-bit register value
void amd64_sub_r32_imm(Reg32 reg, int imm, FILE *file) {
    fputc(0x83, file);              // Opcode
    amd64_r1(reg, 5, file);
    fputc(imm, file);
}

// Subtract an immediate from a 64-bit register value
void amd64_sub_r64_imm(Reg64 reg, int imm, FILE *file) {
    bool extend_reg = reg > RDI;
    amd64_rex_prefix(true, extend_reg, false, file);

    fputc(0x83, file);              // Opcode
    amd64_r1(reg, 5, file);
    fputc(imm, file);
}
