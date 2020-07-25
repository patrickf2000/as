%{
#include <stdio.h>
#include <string.h>

#include <asm.h>

#include "parser.yy.h"
%}

%%
.string                    { return T_STRING; }

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

rax                        { yylval.itype = RAX; return REG64; }
rbx                        { yylval.itype = RBX; return REG64; }
rcx                        { yylval.itype = RCX; return REG64; }
rdx                        { yylval.itype = RDX; return REG64; }
rbp                        { yylval.itype = RBP; return REG64; }
rsp                        { yylval.itype = RSP; return REG64; }
rdi                        { yylval.itype = RDI; return REG64; }
rsi                        { yylval.itype = RSI; return REG64; }

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
[A-Za-z]*([0-9]+)?		   {yylval.stype = strdup(yytext); return ID;}
%%
