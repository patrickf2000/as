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

// Encodes unsigned division on a 32-bit register
void amd64_div_r32(Reg32 src, FILE *file) {
    bool extend_src = src > EDI;
    if (extend_src)
        amd64_rex_prefix(false, extend_src, false, file);

    fputc(0xF7, file);
    amd64_r1(src, 6, file);
}

// Encodes unsigned division on a 64-bit register
void amd64_div_r64(Reg64 src, FILE *file) {
    amd64_write_prefix(src, RAX, file);
    
    fputc(0xF7, file);
    amd64_r1(src, 6, file);
}

// Encodes unsigned division on a 32-bit register
void amd64_div_mem32(Reg64 src, int dsp, FILE *file) {
    bool extend_src = src > RDI;
    if (extend_src)
        amd64_rex_prefix(false, extend_src, false, file);

    fputc(0xF7, file);
    amd64_dsp8(src, dsp, 6, file);
}

// Encodes division on 32-bit register
void amd64_idiv_r32(Reg32 src, FILE *file) {
    bool extend_src = src > EDI;
    if (extend_src)
        amd64_rex_prefix(false, extend_src, false, file);

    fputc(0xF7, file);
    amd64_r1(src, 7, file);
}

// Encodes signed division on a 64-bit register
void amd64_idiv_r64(Reg64 src, FILE *file) {
    amd64_write_prefix(src, RAX, file);
    
    fputc(0xF7, file);
    amd64_r1(src, 7, file);
}

// Encodes division on 32-bit memory location
void amd64_idiv_mem32(Reg64 src, int dsp, FILE *file) {
    bool extend_src = src > RDI;
    if (extend_src)
        amd64_rex_prefix(false, extend_src, false, file);

    fputc(0xF7, file);
    amd64_dsp8(src, dsp, 7, file);
}

// Encodes division on 64-bit memory location
void amd64_idiv_mem64(Reg64 src, int dsp, FILE *file) {
    amd64_write_prefix(src, RAX, file);
    
    fputc(0xF7, file);
    amd64_dsp8(src, dsp, 7, file);
}

