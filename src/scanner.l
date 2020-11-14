%{
#include <stdio.h>
#include <string.h>

#include <asm/asm.hpp>

#include "parser.yy.hpp"
%}

%%
.string                    { return T_STRING; }
.global                    { return GLOBAL; }
extern                     { return EXTERN; }

cmp                        { return CMP; }
call                       { return CALL; }
ret                        { return RET; }
jmp                        { yylval.itype = JMP; return JUMP; }
je                         { yylval.itype = JE;  return JUMP; }
jne                        { yylval.itype = JNE; return JUMP; }
jg                         { yylval.itype = JG;  return JUMP; }
jl                         { yylval.itype = JL;  return JUMP; }
jge                        { yylval.itype = JGE; return JUMP; }
jle                        { yylval.itype = JLE; return JUMP; }
push                       { return PUSH; }
mov                        { return MOV; }
lea                        { return LEA; }
syscall                    { return SYSCALL; }
leave                      { return LEAVE; }

add                        { return ADD; }
sub                        { return SUB; }
imul                       { return IMUL; }

xor                        { return XOR; }

dword                      { return DWORD; }

al                         { yylval.itype = AL; return REG16H; }
cl                         { yylval.itype = CL; return REG16H; }
dl                         { yylval.itype = DL; return REG16H; }
bl                         { yylval.itype = BL; return REG16H; }
ah                         { yylval.itype = AH; return REG16H; }
ch                         { yylval.itype = CH; return REG16H; }
dh                         { yylval.itype = DH; return REG16H; }
bh                         { yylval.itype = BH; return REG16H; }

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

rax                        { yylval.r64type = RAX; return REG64; }
rbx                        { yylval.r64type = RBX; return REG64; }
rcx                        { yylval.r64type = RCX; return REG64; }
rdx                        { yylval.r64type = RDX; return REG64; }
rbp                        { yylval.r64type = RBP; return REG64; }
rsp                        { yylval.r64type = RSP; return REG64; }
rdi                        { yylval.r64type = RDI; return REG64; }
rsi                        { yylval.r64type = RSI; return REG64; }

r8                         { yylval.r64type = R8;  return REG64; }
r9                         { yylval.r64type = R9;  return REG64; }
r10                        { yylval.r64type = R10; return REG64; }
r11                        { yylval.r64type = R11; return REG64; }
r12                        { yylval.r64type = R12; return REG64; }
r13                        { yylval.r64type = R13; return REG64; }
r14                        { yylval.r64type = R14; return REG64; }
r15                        { yylval.r64type = R15; return REG64; }

;.*\n                      {}

"["                        { return '['; }
"]"                        { return ']'; }
","                        { return ','; }
"-"                        { return '-'; }
"+"                        { return '+'; }

\n                         { return NL; }
\"(\\.|[^"\\])*\"          { yylval.stype = strdup(yytext); return STRING;}
[ \t]                      {}
0x[A-Fa-f0-9]*             { yylval.itype = (int)strtol(yytext, NULL, 16); return HEX; }
[-]?[0-9]+                 { yylval.itype = atoi(yytext); return INTEGER;}
[-]?([0-9]*[.])?[0-9]+     { yylval.ftype = atof(yytext); return FLOAT;}
[_]?[A-Za-z]*([0-9]+)?:		   {
                              char *lbl = strdup(yytext);
                              lbl[strlen(lbl)-1] = '\0';
                              yylval.stype = lbl; 
                              return LABEL;
                           }
[_]?[A-Za-z]*([0-9]+)?	   { yylval.stype = strdup(yytext); return ID;}
%%

int yywrap() { return 1; }

