;OUTPUT
;END

;RET 33

section .text
global _start

_start:
    mov rax, 10
    add rax, 5
    
    mov rbx, 8
    add rbx, 10
    
    add rax, rbx
    
    mov rdi, rax
    mov rax, 60
    syscall
