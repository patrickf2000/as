;OUTPUT
;END

;RET 3

section .text
global _start

_start:
    mov eax, 1
    mov ebx, 2
    add eax, ebx
    
    mov edi, eax
    mov eax, 60
    syscall
