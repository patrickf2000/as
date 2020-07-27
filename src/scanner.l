%{
#include <stdio.h>
#include <string.h>

#include <asm.h>

#include "parser.yy.h"
%}

%%
.string                    { return T_STRING; }

cmp                        { return CMP; }
jmp                        { yylval.itype = JMP; return JUMP; }
je                         { yylval.itype = JE;  return JUMP; }
jne                        { yylval.itype = JNE; return JUMP; }
jg                         { yylval.itype = JG;  return JUMP; }
jl                         { yylval.itype = JL;  return JUMP; }
jge                        { yylval.itype = JGE; return JUMP; }
jle                        { yylval.itype = JLE; return JUMP; }
push                       { return PUSH; }
mov                        { return MOV; }
add                        { return ADD; }
sub                        { return SUB; }
syscall                    { return SYSCALL; }

dword                      { return DWORD; }

eax                        { yylval.itype = EAX; return REG32; }
ebx                        { yylval.itype = EBX; return REG32; }
ecx                        { yylval.itype = ECX; return REG32; }
edx                        { yylval.itype = EDX; return REG32; }
ebp                        { yylval.itype = EBP; return REG32; }
esp                        { yylval.itype = ESP; return REG32; }
edi                        { yylval.itype = EDI; return REG32; }
esi                        { yylval.itype = ESI; return REG32; }

r8d                        { yylval.itype = R8D;  return REG32; }
r9d                        { yylval.itype = R9D;  return REG32; }
r10d                       { yylval.itype = R10D; return REG32; }
r11d                       { yylval.itype = R11D; return REG32; }
r12d                       { yylval.itype = R12D; return REG32; }
r13d                       { yylval.itype = R13D; return REG32; }
r14d                       { yylval.itype = R14D; return REG32; }
r15d                       { yylval.itype = R15D; return REG32; }

rax                        { yylval.itype = RAX; return REG64; }
rbx                        { yylval.itype = RBX; return REG64; }
rcx                        { yylval.itype = RCX; return REG64; }
rdx                        { yylval.itype = RDX; return REG64; }
rbp                        { yylval.itype = RBP; return REG64; }
rsp                        { yylval.itype = RSP; return REG64; }
rdi                        { yylval.itype = RDI; return REG64; }
rsi                        { yylval.itype = RSI; return REG64; }

r8                         { yylval.itype = R8;  return REG64; }
r9                         { yylval.itype = R9;  return REG64; }
r10                        { yylval.itype = R10; return REG64; }
r11                        { yylval.itype = R11; return REG64; }
r12                        { yylval.itype = R12; return REG64; }
r13                        { yylval.itype = R13; return REG64; }
r14                        { yylval.itype = R14; return REG64; }
r15                        { yylval.itype = R15; return REG64; }

;.*\n                      {}

"["                        { return '['; }
"]"                        { return ']'; }
","                        { return ','; }
"-"                        { return '-'; }

\n                         { return NL; }
\"(\\.|[^"\\])*\"          {yylval.stype = strdup(yytext); return STRING;}
[ \t]                      {}
[-]?[0-9]+                 {yylval.itype = atoi(yytext); return INTEGER;}
[-]?([0-9]*[.])?[0-9]+     {yylval.ftype = atof(yytext); return FLOAT;}
[A-Za-z]*([0-9]+)?:		   {
                              char *lbl = strdup(yytext);
                              lbl[strlen(lbl)-1] = '\0';
                              yylval.stype = lbl; 
                              return LABEL;
                           }
[A-Za-z]*([0-9]+)?		   {yylval.stype = strdup(yytext); return ID;}
%%
