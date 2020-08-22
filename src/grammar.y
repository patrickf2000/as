%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <asm/asm.h>
#include <elf/elf_bin.h>
#include <utils/sym_table.h>
#include <utils/str_table.h>

extern FILE *yyin;
FILE *file;
int is_pass1 = 0;
int is_sym_pass = 0;

SymbolTable *sym_table;
Elf64_RelaTab *rela_tab;
Elf64_SymTab *elf_sym_table;

char *elf_strtab;
int strtab_start = 0;

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
                                 int len = strlen(elf_strtab);
                                 elf_strtab[len] = '|';
                                     
                                 if (!is_sym_pass) 
                                 {
                                     char *str = strdup($3);
                                     parse_string(str);
                                     
                                     strcat(elf_strtab, str);
                                 } 
                                 else 
                                 {
                                     strcat(elf_strtab, $1);
                                     //int pos = strtab_start + str_table_add($1, elf_strtab);
                                     //elf_add_symbol(elf_sym_table, pos, lc, 1);
                                     sym_table_add(sym_table, $1, lc);
                                 }
                                 
                                 start += strlen($3) - 2;
                                 len = strlen($3) - 2;
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
                        if (is_pass1 && !is_sym_pass) 
                        {
                            if (strcmp($1, "_start") == 0)
                                start = lc;
                            
                            sym_table_add(sym_table, $1, lc);
                        }
                        else if (is_sym_pass)
                        {
                            if (strcmp($1, "_start") != 0)
                            {
                                int pos = strtab_start + str_table_add($1, elf_strtab);
                                elf_add_symbol(elf_sym_table, pos, lc, 0);
                            }
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
      ADD REG32 ',' REG32 NL        { lc += 3; if (!is_pass1) amd64_add_rr32($2, $4, file); }
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
      MOV REG32 ',' REG32 NL                            { 
                                                            lc += 2; 
                                                            if ($2 > EDI || $4 > EDI) ++lc;
                                                            if (!is_pass1) amd64_mov_rr32($2, $4, file); 
                                                        }
    | MOV REG64 ',' REG64 NL                            { lc += 3; if (!is_pass1) amd64_mov_rr64($2, $4, file); }
    | MOV REG32 ',' INTEGER NL                          { 
                                                            lc += 5; 
                                                            if ($2 > EDI || $4 > EDI) ++lc;
                                                            if (!is_pass1) amd64_mov_reg32_imm($2, $4, file); 
                                                        }
    | MOV REG64 ',' INTEGER NL                          { lc += 5; if (!is_pass1) amd64_mov_reg64_imm($2, $4, 0, file); }
    | MOV REG64 ',' ID NL                               { 
                                                          if (is_pass1 && !is_sym_pass) 
                                                          {
                                                              int code_offset = lc + 2;
                                                              int data_offset = sym_table_get(sym_table, $4);
                                                              
                                                              elf_rela_add(rela_tab, code_offset, data_offset);
                                                          }
                                                          else if (!is_pass1 && !is_sym_pass)
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

// Parses a string constant (removes quotes and escape sequences)
void parse_string(char *str)
{
    int length = strlen(str);
    int index = 0;
    
    char *old_str = calloc(length, sizeof(char));
    strcpy(old_str, str);
    memset(str, 0, length);
    
    for (int i = 0; i<length; i++)
    {
        if (old_str[i] == '\"')
        {
            continue;
        }
        else if (old_str[i] == '\\' && old_str[i+1] == 'n')
        {
            str[index] = '\n';
            ++i;
            ++index;
        }
        else
        {
            str[index] = old_str[i];
            ++index;
        }
    }
    
    free(old_str);
}

char *data_parse(const char *path, char *data_values)
{
    elf_strtab = calloc(1024,sizeof(char));

    yyin = fopen(path, "r");
    yyparse();
    
    return elf_strtab;
}

char *data_val_parse(const char *path, char *data_values)
{
    is_sym_pass = 0;
    return data_parse(path, data_values);
}

char *data_name_parse(const char *path, char *data_name, SymbolTable *st)
{
    is_sym_pass = 1;
    sym_table = st;
    lc = 0;
    
    return data_parse(path, data_name);
}

char *symbol_parse(const char *path, char *strtab, Elf64_SymTab *table)
{
    is_pass1 = 1;
    is_sym_pass = 1;
    
    strtab_start = strlen(strtab);
    elf_strtab = calloc(1,sizeof(char));
    
    elf_sym_table = table;

    yyin = fopen(path, "r");
    yyparse();
    
    int length = strlen(strtab) + strlen(elf_strtab) + 1;
    char *new_strtab = calloc(length, sizeof(char));
    strcat(new_strtab, strtab);
    strcat(new_strtab, elf_strtab);
    
    return new_strtab;
}

//Our parsing function
int parse(const char *path, FILE *f, int pass1, SymbolTable *st, Elf64_RelaTab *rt)
{
    sym_table = st;
    rela_tab = rt;

    file = f;
    is_pass1 = pass1;
    is_sym_pass = 0;
    lc = 0;
    start = 0;
    
	yyin = fopen(path, "r");
	yyparse();
    
    return lc;
}

// Returns the start position
int parse_start_pos()
{
    return start;
}

//Handle syntax errors
void yyerror(const char *s)
{
	printf("Syntax error: %s\n", s);
}

int yywrap() { return 1; }
