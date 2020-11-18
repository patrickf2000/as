#pragma once

#include <asm/asm.hpp>

// Move functions
void amd64_mov_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_mov_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_rr(Reg64 r1, Reg64 r2, FILE *file);
void amd64_rr(Reg32 r1, Reg32 r2, FILE *file);
Reg64 amd64_r32_to_r64(Reg32 reg);

// Utility functions
void amd64_rex_prefix(bool size64, bool extend_src, bool extend_dest, FILE *file);
