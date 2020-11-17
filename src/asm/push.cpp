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

void amd64_push_reg64(Reg64 reg, FILE *file)
{
    switch (reg)
    {
        case RAX: fputc(0x50, file); break;
        case RCX: fputc(0x51, file); break;
        case RDX: fputc(0x52, file); break;
        case RBX: fputc(0x53, file); break;
        case RSP: fputc(0x54, file); break;
        case RBP: fputc(0x55, file); break;
        case RSI: fputc(0x56, file); break;
        case RDI: fputc(0x57, file); break;
    }
}
