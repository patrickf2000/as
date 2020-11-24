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

%{
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <elf.h>

#include <asm/asm.hpp>
#include <asm/amd64.hpp>
#include <elf/elf_bin.hpp>

FILE *file;
std::vector<Elf64_Sym> *elf_symtab;
std::vector<Elf64_Rela> *elf_rela_tab;
std::vector<std::string> *strtab;
std::vector<std::string> *data_list;

int lc = 0;
int pass_num = 1;   // Either 1 or 2

std::vector<std::string> global_symbols;
std::map<std::string, int> symtab, extern_tab;

// Bison stuff
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
%}

%define parse.error verbose

%union {
	char* stype;
	int itype;
	float ftype;
	Reg16H r8type;
	Reg16 r16type;
	Reg32 r32type;
	Reg64 r64type;
	Jmp jmp_type;
}

%token T_STRING GLOBAL EXTERN
%token CMP CALL RET PUSH LEA MOV ADD SUB IMUL SYSCALL LEAVE
%token AND XOR SHR
%token DWORD
%token NL

%token <r8type> REG16H
%token <r16type> REG16
%token <r32type> REG32
%token <r64type> REG64
%token <jmp_type> JUMP
%token <itype> INTEGER HEX
%token <ftype> FLOAT
%token <stype> STRING LABEL
%token <stype> ID

%%
all_statements: statement all_statements | /* empty */;

statement:
      data
    | label
    | cmp
    | call
    | ret
    | jmp
    | push
    | add
    | sub
    | imul
    | and
    | xor
    | shift
	| syscall
    | leave
    | lea
    | mov
    | empty
	;

data:
      ID T_STRING STRING NL    {
      							 if (pass_num == 1) {
      							    symtab[$1] = lc;
      							    
      							    std::string str_original = $3;
      							    std::string str = "";
      							    for (int i = 1; i<str_original.length()-2; i++) {
      							        char c = str_original[i];
      							        if (c == '\\') {
      							            c = str_original[i+1];
      							            if (c == 'n') {
      							                str += '\n';
      							            } else {
      							                str += c;
      							            }
      							            
      							            continue;
      							        }
      							        
      							        str += c;
      							    }
      							    
      							    lc += str.length() + 1;
      							    data_list->push_back(str);
      							    
      							    strtab->push_back($1);
      							    int pos = get_str_pos(strtab, $1);
      							    
      							    elf_add_symbol(elf_symtab, pos, lc, 1, 0);
      							 }
                               }
    ;
    
label:
    LABEL NL            {
                            if (pass_num == 1) {
                                int sym_type = 0;   // Local by default
                                for (auto str : global_symbols) {
                                    if (str == $1) {
                                        sym_type = 1;
                                        break;
                                    }
                                }
                                
                                strtab->push_back($1);
                                int pos = get_str_pos(strtab, $1);
                                
                                symtab[$1] = lc;
                                if (sym_type == 1)
                                    elf_add_symbol(elf_symtab, pos, lc, 0, sym_type);
                            }
                        }
    | GLOBAL ID NL      {
                            if (pass_num == 1) {
                               global_symbols.push_back($2); 
                            }
                        }
    | EXTERN ID NL      {
                            if (pass_num == 1) {
                                strtab->push_back($2);
                                int pos = get_str_pos(strtab, $2);
                                
                                int extern_pos = elf_add_symbol(elf_symtab, pos, 0, 0, 2);
                                symtab[$2] = -5;
                                extern_tab[$2] = extern_pos;
                            }
                        }
    ;
    
cmp:
      CMP REG16H ',' INTEGER NL       { lc += 3; if (pass_num == 2) amd64_cmp_reg16h_imm($2, $4, file); }
    | CMP REG16H ',' HEX NL           { lc += 3; if (pass_num == 2) amd64_cmp_reg16h_imm($2, $4, file); }
    | CMP REG32 ',' INTEGER NL        { lc += 3; if (pass_num == 2) amd64_cmp_reg32_imm($2, $4, file); }
    | CMP REG64 ',' INTEGER NL        { lc += 4; if (pass_num == 2) amd64_cmp_reg64_imm($2, $4, file); }
    ;
    
call:
      CALL ID NL    {
                      lc += 5;
                      
                      if (pass_num == 1) {
                          int loco = symtab[$2];
                          if (loco < 0) {
                              int pos = extern_tab[$2];
                              elf_rela_add_func(elf_rela_tab, lc-4, pos);
                          }
                      } else {
                          int loco = symtab[$2];
                          
                          if (loco < 0) {
                              amd64_call(0, file);
                          } else {
                              int pos = loco - lc;
                              amd64_call(pos, file);
                          }
                      }
                    }
    ;
    
ret:
      RET NL        { lc += 1; if (pass_num == 2) amd64_ret(file); }
    ;
    
jmp:
      JUMP ID NL     {
                      lc += 2;
                      if (pass_num == 2)
                      {
                          int loco = symtab[$2];
                          int pos = loco - lc;
                          amd64_jmp($1, pos, file);
                      }
                    }
    ;
    
push:
      PUSH REG64 NL        { ++lc; if (pass_num == 2) amd64_push_reg64($2, file); }
    ;
    
add:
      ADD REG32 ',' REG32 NL                            {
                                                            lc += 2;
                                                            if ($2 > EDI || $4 > EDI) ++lc;
                                                            if (pass_num == 2) amd64_add_rr32($2, $4, file);
                                                        }
    | ADD REG64 ',' REG64 NL                            { lc += 3; if (pass_num == 2) amd64_add_rr64($2, $4, file); }
    | ADD REG32 ',' INTEGER NL                          { lc += 3; if (pass_num == 2) amd64_add_r32_imm($2, $4, file); }
    | ADD REG64 ',' INTEGER NL                          { lc += 4; if (pass_num == 2) amd64_add_r64_imm($2, $4, file); }
    | ADD REG32 ',' '[' REG64 INTEGER ']' NL            { lc += 3; if (pass_num == 2) amd64_add_r32_mem($2, $5, $6, file); }
    | ADD REG64 ',' '[' REG64 INTEGER ']' NL            { lc += 4; if (pass_num == 2) amd64_add_r64_mem($2, $5, $6, file); }
    | ADD DWORD '[' REG64 INTEGER ']' ',' INTEGER NL    { lc += 4; if (pass_num == 2) amd64_add_dw_mem_imm($4, $5, $8, file); }
    ;
    
sub:
      SUB REG32 ',' REG32 NL          {
                                          lc += 2;
                                          if ($2 > EDI || $4 > EDI) ++lc;
                                          if (pass_num == 2) amd64_sub_rr32($2, $4, file);
                                      }
    | SUB REG64 ',' REG64 NL          { lc += 3; if (pass_num == 2) amd64_sub_rr64($2, $4, file); }
    | SUB REG32 ',' INTEGER NL        { lc += 3; if (pass_num == 2) amd64_sub_r32_imm($2, $4, file); }
    | SUB REG64 ',' INTEGER NL        { lc += 4; if (pass_num == 2) amd64_sub_r64_imm($2, $4, file); }
    ;
    
imul:
      IMUL REG32 NL                           {
                                                  lc += 2;
                                                  if ($2 > EDI) ++lc;
                                                  if (pass_num == 2) amd64_imul_r32($2, file);
                                              }
    | IMUL REG64 NL                           { lc += 3; if (pass_num == 2) amd64_imul_r64($2, file); }
    | IMUL REG32 ',' REG32 NL                 {
                                                  lc += 3;
                                                  if ($2 > EDI || $4 > EDI) ++lc;
                                                  if (pass_num == 2) amd64_imul_rr32($2, $4, file);
                                              }
    | IMUL REG64 ',' REG64 NL                 { lc += 4; if (pass_num == 2) amd64_imul_rr64($2, $4, file); }
    | IMUL REG32 ',' INTEGER NL               {
                                                  lc += 6;
                                                  if ($2 > EDI) ++lc;
                                                  if (pass_num == 2) amd64_imul_r32_imm($2, $2, $4, file);
                                              }
    | IMUL REG32 ',' '[' REG64 INTEGER ']' NL {
                                                  lc += 4;
                                                  if ($2 > EDI) ++lc;
                                                  if (pass_num == 2) amd64_imul_r32_mem($2, $5, $6, file);
                                              }
    | IMUL REG32 ',' REG32 ',' INTEGER NL     {
                                                  lc += 6;
                                                  if ($2 > EDI || $4 > EDI) ++lc;
                                                  if (pass_num == 2) amd64_imul_r32_imm($2, $4, $6, file);
                                              }
    | IMUL REG64 ',' INTEGER NL               { lc += 7; if (pass_num == 2) amd64_imul_r64_imm($2, $2, $4, file); }
    | IMUL REG64 ',' REG64 ',' INTEGER NL     { lc += 7; if (pass_num == 2) amd64_imul_r64_imm($2, $4, $6, file); }
    ;

and:
      AND REG32 ',' INTEGER NL      {
                                        lc += 3;
                                        if ($2 > EDI) ++lc;
                                        if (pass_num == 2) amd64_and_r32_imm($2, $4, file);
                                    }
    ;
    
xor:
      XOR REG32 ',' REG32 NL        { lc += 2; if (pass_num == 2) amd64_xor_rr32($2, $4, file); }
    ;
    
shift:
      SHR REG32 ',' INTEGER NL      {
                                        lc += 3;
                                        if ($2 > EDI) ++lc;
                                        if (pass_num == 2) amd64_shr_r32_imm($2, $4, file);
                                    }
    ;
    
syscall:
      SYSCALL NL        { lc += 2; if (pass_num == 2) amd64_syscall(file); }
    ;
    
leave:
      LEAVE NL          { lc += 1; if (pass_num == 2) amd64_leave(file); }
    ;
    
lea:
        LEA REG64 ',' '[' REG64 INTEGER ']' NL              { lc += 4; if (pass_num == 2) amd64_lea64($2, $5, $6, file); }
      | LEA REG64 ',' '[' REG64 INTEGER '*' INTEGER ']' NL  {
                                                                lc += 7;
                                                                if ($2 > RDI || $5 > RDI) ++lc;
                                                                if (pass_num == 2) amd64_lea64_scale($2, $5, $6, $8, file);
                                                            }
    ;
    
mov:
    // ==================================================
    // 16-bit operations
      MOV REG16 ',' REG16 NL                            {
                                                            lc += 3;
                                                            if ($2 > DI || $4 > DI) ++lc;
                                                            if (pass_num == 2) amd64_mov_rr16($2, $4, file);
                                                        }
    | MOV REG16 ',' INTEGER NL                          {
                                                            lc += 4;
                                                            if ($2 > DI) ++lc;
                                                            if (pass_num == 2) amd64_mov_r16_imm($2, $4, file);
                                                        }
    // ==================================================
    // 32-bit operations
    | MOV REG32 ',' REG32 NL                            { 
                                                            lc += 2; 
                                                            if ($2 > EDI || $4 > EDI) ++lc;
                                                            if (pass_num == 2) amd64_mov_rr32($2, $4, file); 
                                                        }
    | MOV REG32 ',' INTEGER NL                          { 
                                                            lc += 5;
                                                            if ($2 > EDI) ++lc;
                                                            if (pass_num == 2) amd64_mov_r32_imm($2, $4, file); 
                                                        }
    | MOV REG32 ',' '[' REG64 INTEGER ']' NL            {
                                                            lc += 3;
                                                            if ($2 > EDI || $5 > RDI) ++lc;
                                                            if (pass_num == 2) amd64_mov_reg32_mem($2, $5, $6, file);
                                                        }
    | MOV REG32 ',' '[' REG64 ']' NL                    {
                                                            lc += 2;
                                                            if ($2 > EDI || $5 > RDI) ++lc;
                                                            if (pass_num == 2) amd64_mov_reg32_mem($2, $5, 0, file);
                                                        }
    | MOV DWORD '[' REG64 INTEGER ']' ',' INTEGER NL    { lc += 7; if (pass_num == 2) amd64_mov_m_int($4, $5, $8, file); }
    | MOV DWORD '[' REG64 ']' ',' INTEGER NL            {
                                                            lc += 6;
                                                            if ($4 > RDI) ++lc;
                                                            if (pass_num == 2) amd64_mov_mem32_imm32($4, 0, $7, file);
                                                        }
    // ==================================================
    // 64-bit operations
    | MOV REG64 ',' REG64 NL                            { lc += 3; if (pass_num == 2) amd64_mov_rr64($2, $4, file); }
    | MOV REG64 ',' INTEGER NL                          { lc += 10; if (pass_num == 2) amd64_mov_r64_imm($2, $4, file); }
    | MOV REG64 ',' ID NL                               { 
                                                          if (pass_num == 1) {
                                                              int code_offset = lc + 2;
                                                              int data_offset = symtab[$4];
                                                              
                                                              elf_rela_add(elf_rela_tab, code_offset, data_offset);
                                                          } else {
                                                              amd64_mov_r64_imm($2, 0, file);
                                                          }
                                                          lc += 10;
                                                        }
    | MOV REG16H ',' '[' REG64 '+' REG64 ']' NL         { lc += 3; if (pass_num == 2) amd64_mov_r8_mrr($2, $5, $7, file); }
    | MOV REG64 ',' '[' REG64 INTEGER ']' NL            { lc += 4; if (pass_num == 2) amd64_mov_reg64_mem($2, $5, $6, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG32 NL            { lc += 3; if (pass_num == 2) amd64_mov_m_reg32($3, $4, $7, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG64 NL            { lc += 4; if (pass_num == 2) amd64_mov_m_reg64($3, $4, $7, file); }
    ;
    
empty:
    NL;

%%

// Pass 1 function for data
int data_pass(std::string data, std::vector<Elf64_Sym> *es, std::vector<std::string> *st, std::vector<std::string> *dt) {
    lc = 0;
    pass_num = 1;
    elf_symtab = es;
    strtab = st;
    data_list = dt;
    
    YY_BUFFER_STATE buffer = yy_scan_string(data.c_str());
    yyparse();
    yy_delete_buffer(buffer);
    return lc;
}

// Pass 1 function
int pass1(std::string data, std::vector<Elf64_Sym> *es, std::vector<std::string> *st, std::vector<Elf64_Rela> *rela_tab) {
    lc = 0;
    pass_num = 1;
    elf_symtab = es;
    strtab = st;
    elf_rela_tab = rela_tab;
    
    YY_BUFFER_STATE buffer = yy_scan_string(data.c_str());
    yyparse();
    yy_delete_buffer(buffer);
    return lc;
}

//Our parsing function
void pass2(std::string data, FILE *f) {
    lc = 0;
    file = f;
    pass_num = 2;
    
    YY_BUFFER_STATE buffer = yy_scan_string(data.c_str());
    yyparse();
    yy_delete_buffer(buffer);
}

//Handle syntax errors
void yyerror(const char *s)
{
	printf("Syntax error: %s\n", s);
	printf("LC at: %d\n", lc);
}

int yywrap() { return 1; }

