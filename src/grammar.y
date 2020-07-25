%{
#include <stdio.h>

#include <asm.h>

extern FILE *yyin;
FILE *file;

int yylex();
void yyerror(const char *s);
%}

%define parse.error verbose

%union {
	char* stype;
	int itype;
	float ftype;
}

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
      push
    | add
    | sub
	| syscall
    | mov
    | empty
	;
    
push:
      PUSH REG64 NL        { amd64_push_reg64($2, file); }
    ;
    
add:
      ADD REG32 ',' REG32 NL        { amd64_add_rr32($2, $4, file); }
    ;
    
sub:
      SUB REG64 ',' INTEGER NL        { amd64_sub_ri($2, $4, file); }
    ;
    
syscall:
      SYSCALL NL        { amd64_syscall(file); }
    ;
    
mov:
      MOV REG64 ',' REG64 NL                            { amd64_mov_rr($2, $4, file); }
    | MOV REG64 ',' INTEGER NL                          { amd64_mov_reg32_imm($2, $4, file); }
    | MOV REG32 ',' '[' REG64 INTEGER ']' NL            { amd64_mov_reg32_mem($2, $5, $6, file); }
    | MOV REG64 ',' '[' REG64 INTEGER ']' NL            { amd64_mov_reg64_mem($2, $5, $6, file); }
    | MOV '[' REG64 INTEGER ']' ',' REG32 NL            { amd64_mov_m_reg32($3, $4, $7, file); }
    | MOV DWORD '[' REG64 INTEGER ']' ',' INTEGER NL    { amd64_mov_m_int($4, $5, $8, file); }
    ;
    
empty:
    NL;

%%

//Our parsing function
void parse(const char *path, FILE *f)
{
    file = f;
    
	yyin = fopen(path, "r");
	yyparse();
}

//Handle syntax errors
void yyerror(const char *s)
{
	printf("Syntax error: %s\n", s);
}

int yywrap() { return 1; }
