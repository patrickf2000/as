;OUTPUT
;END

;RET 100

section .text
global _start

_start:
    mov eax, 100
    mov ebx, 90
    
    sub eax, 20
    sub ebx, 30
    
    add eax, ebx
    sub eax, 40
    
    mov edi, eax
    mov eax, 60
    syscall
