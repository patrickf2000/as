#pragma once

#include <asm/asm.hpp>

// Move functions
void amd64_mov_r16_imm(Reg16 reg, int imm, FILE *file);
void amd64_mov_m16_imm(Reg64 reg, int dsp, short imm, FILE *file);
void amd64_mov_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_mov_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_mov_rr16(Reg16 r1, Reg16 r2, FILE *file);
void amd64_mov_rr32(Reg32 r1, Reg32 r2, FILE *file);
void amd64_mov_rr64(Reg64 r1, Reg64 r2, FILE *file);
void amd64_mov_mem32_imm32(Reg64 dest, int dsp, int imm, FILE *file);
void amd64_mov_mem32_r32(Reg64 dest, int dsp, Reg32 src, FILE *file);
void amd64_mov_m64_imm(Reg64 dest, int dsp, int imm, FILE *file);
void amd64_mov_r64_m64(Reg64 dest, Reg64 src, int dsp, FILE *file);
void amd64_movzx_r32_m16(Reg32 dest, Reg64 src, int dsp, FILE *file);

// LEA instructions
void amd64_lea64(Reg64 dest, Reg64 src, int dsp, FILE *file);
void amd64_lea64_scale(Reg64 dest, Reg64 src, int dsp, int scale, FILE *file);

// Logic functions
void amd64_and_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_and_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_or_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_or_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_xor_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_xor_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_xor_rr64(Reg64 dest, Reg64 src, FILE *file);
void amd64_shl_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_shl_r64_imm(Reg64 reg, int imm, FILE *file);
void amd64_shr_r32_imm(Reg32 reg, int imm, FILE *file);
void amd64_shr_r64_imm(Reg64 reg, int imm, FILE *file);

// Comparison instructions
void amd64_cmp_reg16h_imm(Reg16H op1, int op2, FILE *file);
void amd64_cmp_reg32_imm(Reg32 op1, int op2, FILE *file);
void amd64_cmp_reg64_imm(Reg64 op1, int op2, FILE *file);
void amd64_cmp_rr32(Reg32 dest, Reg32 src, FILE *file);
void amd64_cmp_reg32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file);

// Add and sub instructions
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
void amd64_sub_r32_mem(Reg32 dest, Reg64 src, int dsp, FILE *file);
void amd64_sub_r64_m64(Reg64 dest, Reg64 src, int dsp, FILE *file);

// Signed multiplication instructions
void amd64_imul_rr32(Reg32 dest, Reg32 src, FILE *file);
void amd64_imul_rr64(Reg64 dest, Reg64 src, FILE *file);
void amd64_imul_r32(Reg32 reg, FILE *file);
void amd64_imul_r64(Reg64 reg, FILE *file);
void amd64_imul_r32_imm(Reg32 dest, Reg32 src, int imm, FILE *file);
void amd64_imul_r64_imm(Reg64 dest, Reg64 src, int imm, FILE *file);
void amd64_imul_r32_mem(Reg32 reg, Reg64 src, int dsp, FILE *file);
void amd64_imul_r64_m64(Reg64 dest, Reg64 src, int dsp, FILE *file);

// Division instructions
void amd64_div_r32(Reg32 src, FILE *file);
void amd64_div_mem32(Reg64 src, int dsp, FILE *file);
void amd64_idiv_r32(Reg32 src, FILE *file);
void amd64_idiv_mem32(Reg64 src, int dsp, FILE *file);
void amd64_idiv_mem64(Reg64 src, int dsp, FILE *file);

// Simple functions (no operands)
void amd64_cdqe(FILE *file);
void amd64_syscall(FILE *file);
void amd64_leave(FILE *file);

// Operand functions
void amd64_rex_prefix(bool size64, bool extend_dest, bool extend_src, FILE *file, bool sib=false);
void amd64_r1(Reg64 reg, int digit, FILE *file);
void amd64_r1(Reg32 reg, int digit, FILE *file);
void amd64_rr(Reg64 r1, Reg64 r2, FILE *file);
void amd64_rr(Reg32 r1, Reg32 r2, FILE *file);
void amd64_rr(Reg16 r1, Reg16 r2, FILE *file);
void amd64_dsp8(Reg64 src, int dsp, int digit, FILE *file);
void amd64_rr_dsp8(Reg64 dest, Reg64 src, int dsp, FILE *file);
void amd64_rr_dsp8(Reg32 dest, Reg64 src, int dsp, FILE *file);

// Encoding functions functions
void amd64_dsp0(Reg64 mem, FILE *file);
void amd64_dsp0(Reg64 mem, Reg64 dest, FILE *file);
void amd64_dsp0(Reg64 mem, Reg32 dest, FILE *file);
void amd64_dsp16(Reg64 mem, Reg64 src, int dsp, FILE *file);
void amd64_dsp16(Reg64 mem, Reg32 src, int dsp, FILE *file);
void amd64_sib(Reg64 mem, Reg64 src, int dsp, int scale, FILE *file);
void amd64_mem_imm(Reg64 dest, int dsp, FILE *file);

// Utility functions
void amd64_write_prefix(int size, Reg64 r1, Reg64 r2, FILE *file);
void amd64_write_prefix(Reg64 r1, Reg64 r2, FILE *file);
void amd64_write_prefix(Reg32 r1, Reg32 r2, FILE *file);
void amd64_write_prefix(Reg32 r1, Reg64 r2, FILE *file);
void amd64_write_prefix(Reg16 r1, Reg16 r2, FILE *file);
Reg64 amd64_r16_to_r64(Reg16 reg);
Reg64 amd64_r32_to_r64(Reg32 reg);

