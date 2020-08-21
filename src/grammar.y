%{
#include <stdio.h>
#include <string.h>

#include <asm/asm.h>
#include <utils/sym_table.h>
#include <elf/elf_bin.h>

extern FILE *yyin;
FILE *file;
int is_pass1 = 0;

SymbolTable *sym_table;
Elf64_RelaTab *rela_tab;

int start = 0;
int lc = 0;
const int code_start = 4194424;

int yylex();
void yyerror(const char *s);
%}

%define parse.error verbose

%union {
	char* stype;
	int itype;
	float ftype;
}

%token T_STRING
%token CMP CALL RET PUSH MOV ADD SUB SYSCALL LEAVE
%token XOR
%token DWORD
%token NL

%token <itype> INTEGER HEX REG16H REG32 REG64 JUMP
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
    | xor
	| syscall
    | leave
    | mov
    | empty
	;
    
data:
      ID T_STRING STRING NL    { 
                                 if (is_pass1) 
                                 {
                                     sym_table_add(sym_table, $1, start);
                                 } 
                                 else 
                                 {
                                     amd64_write_string($3, file);
                                 }
                                 
                                 start += strlen($3) - 2;
                                 int len = strlen($3) - 2;
                                 if (strstr($3, "\\n") != NULL)
                                 {
                                     start -= 1;
                                     len -= 1;
                                 }
                                 lc += len;
                               }
    ;
    
label:
    LABEL NL        {
                        if (is_pass1) 
                        {
                            if (strcmp($1, "_start") == 0)
                                start = lc;
                            
                            sym_table_add(sym_table, $1, lc);
                        }
                    }
    ;
    
cmp:
      CMP REG16H ',' INTEGER NL       { lc += 3; if (!is_pass1) amd64_cmp_reg16h_imm($2, $4, file); }
    | CMP REG16H ',' HEX NL           { lc += 3; if (!is_pass1) amd64_cmp_reg16h_imm($2, $4, file); }
    | CMP REG32 ',' INTEGER NL        { lc += 3; if (!is_pass1) amd64_cmp_reg32_imm($2, $4, file); }
    ;
    
call:
      CALL ID NL    {
                      lc += 5;
                      if (!is_pass1)
                      {
                          int loco = sym_table_get(sym_table, $2);
                          int pos = loco - lc;
                          amd64_call(pos, file);
                      }
                    }
    ;
    
ret:
      RET NL        { lc += 1; if (!is_pass1) amd64_ret(file); }
    ;
    
jmp:
      JUMP ID NL     {
                      lc += 2;
                      if (!is_pass1)
                      {
                          int loco = sym_table_get(sym_table, $2);
                          int pos = loco - lc;
                          amd64_jmp($1, pos, file);
                      }
                    }
    ;
    
push:
      PUSH REG64 NL        { ++lc; if (!is_pass1) amd64_push_reg64($2, file); }
    ;
    
add:
      ADD REG32 ',' REG32 NL        { lc += 2; if (!is_pass1) amd64_add_rr32($2, $4, file); }
    | ADD REG32 ',' INTEGER NL      { lc += 3; if (!is_pass1) amd64_add_r32_imm($2, $4, file); }
    ;
    
sub:
      SUB REG64 ',' INTEGER NL        { lc += 4; if (!is_pass1) amd64_sub_ri($2, $4, file); }
    ;
    
xor:
      XOR REG32 ',' REG32 NL        { lc += 2; if (!is_pass1) amd64_xor_rr32($2, $4, file); }
    ;
    
syscall:
      SYSCALL NL        { lc += 2; if (!is_pass1) amd64_syscall(file); }
    ;
    
leave:
      LEAVE NL          { lc += 1; if (!is_pass1) amd64_leave(file); }
    ;
    
mov:
      MOV REG32 ',' REG32 NL                            { lc += 2; if (!is_pass1) amd64_mov_rr32($2, $4, file); }
    | MOV REG64 ',' REG64 NL                            { lc += 3; if (!is_pass1) amd64_mov_rr64($2, $4, file); }
    | MOV REG32 ',' INTEGER NL                          { lc += 5; if (!is_pass1) amd64_mov_reg32_imm($2, $4, file); }
    | MOV REG64 ',' INTEGER NL                          { lc += 5; if (!is_pass1) amd64_mov_reg64_imm($2, $4, 0, file); }
    | MOV REG64 ',' ID NL                               { 
                                                          if (is_pass1) 
                                                          {
                                                              int code_offset = lc + 2;
                                                              int data_offset = sym_table_get(sym_table, $4);
                                                              
                                                              elf_rela_add(rela_tab, code_offset, data_offset);
                                                          }
                                                          else
                                                          {
                                                              amd64_mov_reg64_imm($2, 0, 1, file);
                                                              
                                                          }
                                                          lc += 10;
                                                        }
    | MOV REG16H ',' '[' REG64 '+' REG64 ']' NL         { lc += 3; if (!is_pass1) amd64_mov_r8_mrr($2, $5, $7, file); }
    | MOV REG32 ',' '[' REG64 INTEGER ']' NL            { lc += 3; if (!is_pass1) amd64_mov_reg32_mem($2, $5, $6, file); }
    | MOV REG64 ',' '[' REG64 INTEGER ']' NL            { lc += 4; if (!is_pass1) amd64_mov_reg64_mem($2, $5, $6, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG32 NL            { lc += 3; if (!is_pass1) amd64_mov_m_reg32($3, $4, $7, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG64 NL            { lc += 4; if (!is_pass1) amd64_mov_m_reg64($3, $4, $7, file); }
    | MOV DWORD '[' REG64 INTEGER ']' ',' INTEGER NL    { lc += 7; if (!is_pass1) amd64_mov_m_int($4, $5, $8, file); }
    ;
    
empty:
    NL;

%%

//Our parsing function
int parse(const char *path, FILE *f, int pass1, SymbolTable *st, Elf64_RelaTab *rt)
{
    sym_table = st;
    rela_tab = rt;

    file = f;
    is_pass1 = pass1;
    lc = 0;
    start = 0;
    
	yyin = fopen(path, "r");
	yyparse();
    
    return lc;
}

//Handle syntax errors
void yyerror(const char *s)
{
	printf("Syntax error: %s\n", s);
}

int yywrap() { return 1; }
