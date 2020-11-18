%{
#include <stdio.h>
#include <string.h>

#include <asm/asm.hpp>

#include "parser.yy.hpp"
%}

%%
.string                    { return T_STRING; }
global                     { return GLOBAL; }
extern                     { return EXTERN; }

cmp                        { return CMP; }
call                       { return CALL; }
ret                        { return RET; }
jmp                        { yylval.jmp_type = JMP; return JUMP; }
je                         { yylval.jmp_type = JE;  return JUMP; }
jne                        { yylval.jmp_type = JNE; return JUMP; }
jg                         { yylval.jmp_type = JG;  return JUMP; }
jl                         { yylval.jmp_type = JL;  return JUMP; }
jge                        { yylval.jmp_type = JGE; return JUMP; }
jle                        { yylval.jmp_type = JLE; return JUMP; }
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

al                         { yylval.r8type = AL; return REG16H; }
cl                         { yylval.r8type = CL; return REG16H; }
dl                         { yylval.r8type = DL; return REG16H; }
bl                         { yylval.r8type = BL; return REG16H; }
ah                         { yylval.r8type = AH; return REG16H; }
ch                         { yylval.r8type = CH; return REG16H; }
dh                         { yylval.r8type = DH; return REG16H; }
bh                         { yylval.r8type = BH; return REG16H; }

ax                         { yylval.r16type = AX; return REG16; }
bx                         { yylval.r16type = BX; return REG16; }
cx                         { yylval.r16type = CX; return REG16; }
dx                         { yylval.r16type = DX; return REG16; }
bp                         { yylval.r16type = BP; return REG16; }
sp                         { yylval.r16type = SP; return REG16; }
di                         { yylval.r16type = DI; return REG16; }
si                         { yylval.r16type = SI; return REG16; }

r8w                        { yylval.r16type = R8W; return REG16; }
r9w                        { yylval.r16type = R9W; return REG16; }
r10w                       { yylval.r16type = R10W; return REG16; }
r11w                       { yylval.r16type = R11W; return REG16; }
r12w                       { yylval.r16type = R12W; return REG16; }
r13w                       { yylval.r16type = R13W; return REG16; }
r14w                       { yylval.r16type = R14W; return REG16; }
r15w                       { yylval.r16type = R15W; return REG16; }

eax                        { yylval.r32type = EAX; return REG32; }
ebx                        { yylval.r32type = EBX; return REG32; }
ecx                        { yylval.r32type = ECX; return REG32; }
edx                        { yylval.r32type = EDX; return REG32; }
ebp                        { yylval.r32type = EBP; return REG32; }
esp                        { yylval.r32type = ESP; return REG32; }
edi                        { yylval.r32type = EDI; return REG32; }
esi                        { yylval.r32type = ESI; return REG32; }

r8d                        { yylval.r32type = R8D;  return REG32; }
r9d                        { yylval.r32type = R9D;  return REG32; }
r10d                       { yylval.r32type = R10D; return REG32; }
r11d                       { yylval.r32type = R11D; return REG32; }
r12d                       { yylval.r32type = R12D; return REG32; }
r13d                       { yylval.r32type = R13D; return REG32; }
r14d                       { yylval.r32type = R14D; return REG32; }
r15d                       { yylval.r32type = R15D; return REG32; }

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
[-+]?[0-9]+                { yylval.itype = atoi(yytext); return INTEGER;}
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

