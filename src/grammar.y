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
#include <string>

#include <asm/asm.hpp>

FILE *file;
int lc = 0;
int pass_num = 1;   // Either 1 or 2

// Bison stuff
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
int yylex();
void yyerror(const char *s);
%}

%define parse.error verbose

%union {
	char* stype;
	int itype;
	float ftype;
	Reg64 r64type;
}

%token T_STRING GLOBAL EXTERN
%token CMP CALL RET PUSH LEA MOV ADD SUB IMUL SYSCALL LEAVE
%token XOR
%token DWORD
%token NL

%token <r64type> REG64
%token <itype> INTEGER HEX REG16H REG32 JUMP
%token <ftype> FLOAT
%token <stype> STRING LABEL
%token <stype> ID

%%
all_statements: statement all_statements | /* empty */;

statement:
      label
    | cmp
    | call
    | ret
    | jmp
    | push
    | add
    | sub
    | imul
    | xor
	| syscall
    | leave
    | lea
    | mov
    | empty
	;
    
label:
    LABEL NL            {
                        }
    | GLOBAL ID NL      {
                        }
    | EXTERN ID NL      {
                        }
    ;
    
cmp:
      CMP REG16H ',' INTEGER NL       { lc += 3; if (pass_num == 2) {} }//amd64_cmp_reg16h_imm($2, $4, file); }
    | CMP REG16H ',' HEX NL           { lc += 3; if (pass_num == 2) {} }//amd64_cmp_reg16h_imm($2, $4, file); }
    | CMP REG32 ',' INTEGER NL        { lc += 3; if (pass_num == 2) {} }//amd64_cmp_reg32_imm($2, $4, file); }
    | CMP REG64 ',' INTEGER NL        { lc += 4; if (pass_num == 2) {} }//amd64_cmp_reg64_imm($2, $4, file); }
    ;
    
call:
      CALL ID NL    {
                      lc += 5;
                      
                      /*if (pass_type == Build1)
                      {
                          int loco = sym_table_get(sym_table, $2);
                          
                          if (loco == -5)
                          {
                              int pos = sym_table_get(extern_table, $2);
                              
                              elf_rela_add_func(rela_tab, lc-4, pos);
                          }
                      }
                      else if (pass_num == 2) {}
                      {
                          int loco = sym_table_get(sym_table, $2);
                          
                          if (loco == -5)
                          {
                              amd64_call(0, file);
                          }
                          else
                          {
                              int pos = loco - lc;
                              amd64_call(pos, file);
                          }
                      }*/
                    }
    ;
    
ret:
      RET NL        { lc += 1; if (pass_num == 2) {} }//amd64_ret(file); }
    ;
    
jmp:
      JUMP ID NL     {
                      lc += 2;
                      if (pass_num == 2)
                      {
                          /*int loco = sym_table_get(sym_table, $2);
                          int pos = loco - lc;
                          amd64_jmp($1, pos, file);*/
                      }
                    }
    ;
    
push:
      PUSH REG64 NL        { ++lc; if (pass_num == 2) {} }//amd64_push_reg64($2, file); }
    ;
    
add:
      ADD REG32 ',' REG32 NL                            { lc += 3; if (pass_num == 2) {} }//amd64_add_rr32($2, $4, file); }
    | ADD REG64 ',' REG64 NL                            { lc += 3; if (pass_num == 2) {} }//amd64_add_rr64($2, $4, file); }
    | ADD REG32 ',' INTEGER NL                          { lc += 3; if (pass_num == 2) {} }//amd64_add_r32_imm($2, $4, file); }
    | ADD REG64 ',' INTEGER NL                          { lc += 4; if (pass_num == 2) {} }//amd64_add_r64_imm($2, $4, file); }
    | ADD REG32 ',' '[' REG64 INTEGER ']' NL            { lc += 3; if (pass_num == 2) {} }//amd64_add_r32_mem($2, $5, $6, file); }
    | ADD REG64 ',' '[' REG64 INTEGER ']' NL            { lc += 4; if (pass_num == 2) {} }//amd64_add_r64_mem($2, $5, $6, file); }
    | ADD DWORD '[' REG64 INTEGER ']' ',' INTEGER NL    { lc += 4; if (pass_num == 2) {} }//amd64_add_dw_mem_imm($4, $5, $8, file); }
    ;
    
sub:
      SUB REG32 ',' REG32 NL          { lc += 3; if (pass_num == 2) {} }//amd64_sub_rr32($2, $4, file); }
    | SUB REG64 ',' REG64 NL          { lc += 3; if (pass_num == 2) {} }//amd64_sub_rr64($2, $4, file); }
    | SUB REG32 ',' INTEGER NL        { lc += 3; if (pass_num == 2) {} }//amd64_sub_r32_imm($2, $4, file); }
    | SUB REG64 ',' INTEGER NL        { lc += 4; if (pass_num == 2) {} }//amd64_sub_r64_imm($2, $4, file); }
    ;
    
imul:
      IMUL REG32 NL                           {
                                                  lc += 2;
                                                  if ($2 > EDI) ++lc;
                                                  if (pass_num == 2) {} //amd64_imul_r32($2, file);
                                              }
    | IMUL REG64 NL                           { lc += 3; if (pass_num == 2) {} }//amd64_imul_r64($2, file); }
    | IMUL REG32 ',' REG32 NL                 {
                                                  lc += 3;
                                                  if ($2 > EDI || $4 > EDI) ++lc;
                                                  if (pass_num == 2) {} //amd64_imul_rr32($2, $4, file);
                                              }
    | IMUL REG64 ',' REG64 NL                 { lc += 4; if (pass_num == 2) {} }//amd64_imul_rr64($2, $4, file); }
    | IMUL REG32 ',' INTEGER NL               {
                                                  lc += 6;
                                                  if ($2 > EDI) ++lc;
                                                  if (pass_num == 2) {} //amd64_imul_r32_imm($2, $2, $4, file);
                                              }
    | IMUL REG32 ',' REG32 ',' INTEGER NL     {
                                                  lc += 6;
                                                  if ($2 > EDI || $4 > EDI) ++lc;
                                                  if (pass_num == 2) {} //amd64_imul_r32_imm($2, $4, $6, file);
                                              }
    | IMUL REG64 ',' INTEGER NL               { lc += 7; if (pass_num == 2) {} }//amd64_imul_r64_imm($2, $2, $4, file); }
    | IMUL REG64 ',' REG64 ',' INTEGER NL     { lc += 7; if (pass_num == 2) {} }//amd64_imul_r64_imm($2, $4, $6, file); }
    ;
    
xor:
      XOR REG32 ',' REG32 NL        { lc += 2; if (pass_num == 2) {} }//amd64_xor_rr32($2, $4, file); }
    ;
    
syscall:
      SYSCALL NL        { lc += 2; if (pass_num == 2) amd64_syscall(file); }
    ;
    
leave:
      LEAVE NL          { lc += 1; if (pass_num == 2) {} }//amd64_leave(file); }
    ;
    
lea:
      LEA REG64 ',' '[' REG64 INTEGER ']' NL            { lc += 4; if (pass_num == 2) {} }//amd64_lea64($2, $5, $6, file); }
    ;
    
mov:
      MOV REG32 ',' REG32 NL                            { 
                                                            lc += 2; 
                                                            if ($2 > EDI || $4 > EDI) ++lc;
                                                            if (pass_num == 2) {} //amd64_mov_rr32($2, $4, file); 
                                                        }
    | MOV REG64 ',' REG64 NL                            { lc += 3; if (pass_num == 2) {} }//amd64_mov_rr64($2, $4, file); }
    | MOV REG32 ',' INTEGER NL                          { 
                                                            lc += 5; 
                                                            if ($2 > EDI) ++lc;
                                                            if (pass_num == 2) {} //amd64_mov_reg32_imm($2, $4, file); 
                                                        }
    | MOV REG64 ',' INTEGER NL                          { lc += 10; if (pass_num == 2) amd64_mov_reg64_imm($2, $4, file); }
    | MOV REG64 ',' ID NL                               { 
                                                          /*if (pass_type == Build1) 
                                                          {
                                                              int code_offset = lc + 2;
                                                              int data_offset = sym_table_get(sym_table, $4);
                                                              
                                                              elf_rela_add(rela_tab, code_offset, data_offset);
                                                          }
                                                          else if (pass_num == 2) {}
                                                          {
                                                              amd64_mov_reg64_imm($2, 0, file);
                                                              
                                                          }*/
                                                          lc += 10;
                                                        }
    | MOV REG16H ',' '[' REG64 '+' REG64 ']' NL         { lc += 3; if (pass_num == 2) {} }//amd64_mov_r8_mrr($2, $5, $7, file); }
    | MOV REG32 ',' '[' REG64 INTEGER ']' NL            { lc += 3; if (pass_num == 2) {} }//amd64_mov_reg32_mem($2, $5, $6, file); }
    | MOV REG64 ',' '[' REG64 INTEGER ']' NL            { lc += 4; if (pass_num == 2) {} }//amd64_mov_reg64_mem($2, $5, $6, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG32 NL            { lc += 3; if (pass_num == 2) {} }//amd64_mov_m_reg32($3, $4, $7, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG64 NL            { lc += 4; if (pass_num == 2) {} }//amd64_mov_m_reg64($3, $4, $7, file); }
    | MOV DWORD '[' REG64 INTEGER ']' ',' INTEGER NL    { lc += 7; if (pass_num == 2) {} }//amd64_mov_m_int($4, $5, $8, file); }
    ;
    
empty:
    NL;

%%

//Our parsing function
int parse(std::string data, int pass_num, FILE *f) {
    file = f;
    
    YY_BUFFER_STATE buffer = yy_scan_string(data.c_str());
    yyparse();
    yy_delete_buffer(buffer);
    return lc;
}

//Handle syntax errors
void yyerror(const char *s)
{
	printf("Syntax error: %s\n", s);
}

int yywrap() { return 1; }

