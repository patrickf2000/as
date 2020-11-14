;OUTPUT
;END

;RET 144

section .text
global _start

_start:
    mov rax, 2
    mov rbx, 3
    mov rcx, 4
    
    imul rbx
    imul rcx
    
    mov r15, 6
    mov r14, 1
    
    imul r15
    imul r14
    
    mov rdi, rax
    
    mov rax, 60
    syscall

    
