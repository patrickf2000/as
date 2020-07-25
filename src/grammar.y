%{
#include <stdio.h>
#include <string.h>

#include <asm.h>
#include <sym_table.h>

extern FILE *yyin;
FILE *file;
int is_pass1 = 0;

SymbolTable *sym_table;
int start = 0;
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
%token PUSH MOV ADD SUB SYSCALL
%token DWORD
%token NL

%token <itype> INTEGER REG32 REG64
%token <ftype> FLOAT
%token <stype> STRING
%token <stype> ID

%%
all_statements: statement all_statements | /* empty */;

statement:
      data
    | push
    | add
    | sub
	| syscall
    | mov
    | empty
	;
    
data:
      ID T_STRING STRING NL    { 
                                 if (is_pass1) 
                                 {
                                     sym_table_add(sym_table, $1, start);
                                     start += strlen($3) - 2;
                                 } 
                                 else 
                                 {
                                     amd64_write_string($3, file);
                                 }
                               }
    ;
    
push:
      PUSH REG64 NL        { if (!is_pass1) amd64_push_reg64($2, file); }
    ;
    
add:
      ADD REG32 ',' REG32 NL        { if (!is_pass1) amd64_add_rr32($2, $4, file); }
    ;
    
sub:
      SUB REG64 ',' INTEGER NL        { if (!is_pass1) amd64_sub_ri($2, $4, file); }
    ;
    
syscall:
      SYSCALL NL        { if (!is_pass1) amd64_syscall(file); }
    ;
    
mov:
      MOV REG64 ',' REG64 NL                            { if (!is_pass1) amd64_mov_rr($2, $4, file); }
    | MOV REG64 ',' INTEGER NL                          { if (!is_pass1) amd64_mov_reg32_imm($2, $4, file); }
    | MOV REG64 ',' ID NL                               { 
                                                          if (!is_pass1) 
                                                          {
                                                              int loco = sym_table_get(sym_table, $4) + code_start;
                                                              amd64_mov_reg32_imm($2, loco, file);
                                                          }
                                                        }
    | MOV REG32 ',' '[' REG64 INTEGER ']' NL            { if (!is_pass1) amd64_mov_reg32_mem($2, $5, $6, file); }
    | MOV REG64 ',' '[' REG64 INTEGER ']' NL            { if (!is_pass1) amd64_mov_reg64_mem($2, $5, $6, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG32 NL            { if (!is_pass1) amd64_mov_m_reg32($3, $4, $7, file); }
    | MOV DWORD '[' REG64 INTEGER ']' ',' INTEGER NL    { if (!is_pass1) amd64_mov_m_int($4, $5, $8, file); }
    ;
    
empty:
    NL;

%%

//Our parsing function
int parse(const char *path, FILE *f, int pass1, SymbolTable *st)
{
    sym_table = st;

    file = f;
    is_pass1 = pass1;
    
	yyin = fopen(path, "r");
	yyparse();
    
    return start;
}

//Handle syntax errors
void yyerror(const char *s)
{
	printf("Syntax error: %s\n", s);
}

int yywrap() { return 1; }
